#include "store/profile/profile_store.hpp"

#include <cctype>
#include <format>
#include <ranges>
#include <utility>

#include "domain/profile.hpp"
#include "logging/log_macros.hpp"
#include "service/i_profile_service.hpp"
#include "store/base/base_store.hpp"
#include "store/profile/exception.hpp"

REGISTER_LOG_CATEGORY("Store.ProfileStore");

namespace store
{
    namespace
    {
        ProfileStoreResult toProfileStoreResult(StoreResult result)
        {
            switch (result)
            {
                case StoreResult::Ok:
                    return ProfileStoreResult::Ok;
                case StoreResult::NotFound:
                    return ProfileStoreResult::ProfileNotFound;
            }

            std::unreachable();
        }
    }   // namespace

    /**
     * @brief Construct a new Profile Store:: Profile Store object
     *
     * @note It loads all profiles from the underlying service upon
     * construction. Additionally, it initializes the internal state tracking
     * for each profile.
     *
     * @param profileService
     */
    ProfileStore::ProfileStore(
        const std::shared_ptr<service::IProfileService>& profileService
    )
        : _profileService{profileService}
    {
        const auto profiles = _profileService->getAll();

        _addCleanEntries(profiles);
    }

    /**
     * @brief Check if there are any profiles in the store
     *
     * @note This ignores profiles marked as deleted
     *
     * @return true
     * @return false
     */
    bool ProfileStore::hasProfiles() const { return _hasNonDeletedEntries(); }

    /**
     * @brief Get a list of all profile names in the store
     *
     * @note This ignores profiles marked as deleted
     *
     * @return std::vector<std::string>
     */
    std::vector<std::string> ProfileStore::getAllProfileNames() const
    {
        const auto options = Options{.deletion = DeletionPolicy::ExcludeDelete};

        auto profiles = _getValues(options) |
                        std::views::transform([](const auto& profile)
                                              { return profile.getName(); });

        return {profiles.begin(), profiles.end()};
    }

    /**
     * @brief Set the active profile by name
     *
     * @param name
     *
     * @return ProfileStoreResult
     */
    ProfileStoreResult ProfileStore::setActiveProfile(std::string_view name)
    {
        const auto profile = _getProfile(name);

        if (!profile)
        {
            LOG_ERROR(std::format("Profile '{}' not found", name));
            return ProfileStoreResult::ProfileNotFound;
        }

        _activeProfile.set(profile->getId());
        return ProfileStoreResult::Ok;
    }

    /**
     * @brief Get the currently active profile
     *
     * @return std::optional<domain::Profile>
     */
    std::optional<domain::Profile> ProfileStore::getActiveProfile() const
    {
        if (!_activeProfile.has())
            return std::nullopt;

        return _getProfile(_activeProfile.get().value());
    }

    /**
     * @brief Get a profile by its ID
     *
     * @note Ignores profiles marked as deleted
     *
     * @param id
     * @return std::optional<domain::Profile>
     */
    std::optional<domain::Profile> ProfileStore::_getProfile(ProfileId id) const
    {
        return _get(
            {.filter   = domain::HasProfileId(id),
             .deletion = DeletionPolicy::ExcludeDelete}
        );
    }

    /**
     * @brief Get a profile by its name
     *
     * @note Ignores profiles marked as deleted
     *
     * @param name
     * @return std::optional<domain::Profile>
     */
    std::optional<domain::Profile> ProfileStore::_getProfile(
        std::string_view name
    ) const
    {
        return _get(
            {.filter   = domain::HasProfileName(name),
             .deletion = DeletionPolicy::ExcludeDelete}
        );
    }

    /**
     * @brief Check if a profile with the given name exists
     *
     * @note Ignores profiles marked as deleted
     *
     * @param name
     * @return true
     * @return false
     */
    bool ProfileStore::profileExists(std::string_view name) const
    {
        return _getProfile(name).has_value();
    }

    /**
     * @brief Add a new profile to the store
     *
     * @note If a profile with the same name exists and is marked as
     * deleted, it will be restored and updated with the new draft
     * information. Otherwise, a new profile will be created.
     *
     * @param draft
     * @return ProfileStoreResult
     */
    ProfileStoreResult ProfileStore::addProfile(const domain::Profile& draft)
    {
        auto entry = _getEntry(
            {.filter   = domain::HasProfileName(draft.getName()),
             .deletion = DeletionPolicy::IncludeDelete}
        );

        if (entry.has_value())
        {
            if (entry->state != StoreState::Deleted)
                return ProfileStoreResult::NameAlreadyExists;

            entry->value.setName(draft.getName());
            entry->value.setEmail(draft.getEmail());
            const auto result =
                _updateEntry(entry->value, StoreState::Modified);

            return toProfileStoreResult(result);
        }

        // NOTE: here we can use an invalid ProfileId as the ID will get
        // overwritten
        _addEntry(
            domain::Profile{
                ProfileId::invalid(),
                draft.getName(),
                draft.getEmail()
            }
        );

        return ProfileStoreResult::Ok;
    }

    /**
     * @brief Remove a profile from the store
     *
     * @note Marks the profile as deleted; actual removal occurs during
     * commit
     *
     * @param draft
     * @return ProfileStoreResult
     */
    ProfileStoreResult ProfileStore::removeProfile(const domain::Profile& draft)
    {
        const auto entry = _getEntry(
            {.filter   = domain::HasProfileName(draft.getName()),
             .deletion = DeletionPolicy::IncludeDelete}
        );

        if (!entry.has_value())
            return ProfileStoreResult::ProfileNotFound;

        if (_activeProfile.has() &&
            entry->value.getId() == _activeProfile.get().value())
        {
            _activeProfile.unset();
        }

        const auto result = _deleteEntry(entry->value.getId());

        return toProfileStoreResult(result);
    }

    /**
     * @brief Commit a new profile to the underlying service
     *
     * @param entry
     */
    void ProfileStore::_commitNewProfile(Entry entry)
    {
        const auto oldId = entry.value.getId();
        const auto newId = _profileService->create(entry.value);

        entry.value.setId(newId);

        const auto result = _commitEntry(oldId, entry);

        if (result != StoreResult::Ok)
        {
            throw ProfileStoreException(
                std::format(
                    "Failed to commit new profile {} with error {}",
                    entry.value.getName(),
                    StoreResultMeta::toString(result)
                )
            );
        }

        // Update the active profile ID if it was pointing to the old ID
        if (_activeProfile.has() && _activeProfile.get().value() == oldId)
            _activeProfile.set(newId);

        LOG_INFO(
            std::format("Profile '{}' saved to database", entry.value.getName())
        );
    }

    /**
     * @brief Commit a modified profile to the underlying service
     *
     * @param entry
     */
    void ProfileStore::_commitModifiedProfile(const Entry& entry)
    {
        const auto& name  = entry.value.getName();
        const auto& email = entry.value.getEmail();
        const auto  id    = entry.value.getId();

        _profileService->update(id, name, email);
        const auto result = _commitEntry(id, entry);

        if (result != StoreResult::Ok)
        {
            throw ProfileStoreException(
                std::format(
                    "Failed to commit modified profile {} with error {}",
                    name,
                    StoreResultMeta::toString(result)
                )
            );
        }

        LOG_INFO(
            std::format(
                "Profile '{}' updated in database",
                entry.value.getName()
            )
        );
    }

    /**
     * @brief Commit a deleted profile to the underlying service
     *
     * @param entry
     */
    void ProfileStore::_commitDeletedProfile(const Entry& entry)
    {
        const auto id = entry.value.getId();

        _profileService->remove(id);
        const auto result = _commitEntry(id, entry);

        if (result != StoreResult::Ok)
        {
            throw ProfileStoreException(
                std::format(
                    "Failed to commit deleted profile {} with error {}",
                    entry.value.getName(),
                    StoreResultMeta::toString(result)
                )
            );
        }

        LOG_INFO(
            std::format(
                "Profile '{}' removed from database",
                entry.value.getName()
            )
        );
    }

    /**
     * @brief Commit all pending changes to the underlying service
     *
     */
    void ProfileStore::commit()
    {
        for (const auto& entry : _getEntries())
        {
            switch (entry.state)
            {
                case StoreState::New:
                    _commitNewProfile(entry);
                    break;
                case StoreState::Modified:
                    _commitModifiedProfile(entry);
                    break;
                case StoreState::Deleted:
                    _commitDeletedProfile(entry);
                    break;
                case StoreState::Clean:
                    break;
            }
        }
    }

    /**
     * @brief Subscribe to profile change events
     *
     * @param func
     * @param user
     * @return Connection
     */
    Connection ProfileStore::subscribeToProfileChange(
        const OnProfileChanged::func& func,
        void*                         user
    )
    {
        return _activeProfile.on<OnProfileChanged>(func, user);
    }

}   // namespace store