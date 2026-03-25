#include "app/store/profile/profile_store.hpp"

#include <cctype>
#include <format>

#include "app/domain/profile.hpp"
#include "app/services_api/i_profile_service.hpp"
#include "app/store/profile/exception.hpp"
#include "app/store/profile/predicates.hpp"
#include "drafts/profile_draft.hpp"
#include "logging/log_macros.hpp"

REGISTER_LOG_CATEGORY("App.Store.ProfileStore");

namespace app
{
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
        const std::shared_ptr<IProfileService>& profileService
    )
        : _profileService{profileService}
    {
        const auto profiles = _profileService->getAll();

        for (const auto& profile : profiles)
            _addEntry(profile, StoreState::Clean);
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
        std::vector<std::string> names;

        for (const auto& entr : _getEntries())
        {
            if (entr.state == StoreState::Deleted)
                continue;

            names.push_back(entr.value.getName());
        }

        return names;
    }

    /**
     * @brief Set the active profile by name
     *
     * @param name
     */
    void ProfileStore::setActiveProfile(std::string_view name)
    {
        const auto profile = getProfile(name);
        if (!profile)
        {
            throw ProfileStoreException(
                std::format("Profile '{}' not found", name)
            );
        }

        _activeProfile.set(profile->getId());
    }

    /**
     * @brief Check if there is an active profile set
     *
     * @return true
     * @return false
     */
    bool ProfileStore::hasActiveProfile() const { return _activeProfile.has(); }

    /**
     * @brief Get the currently active profile
     *
     * @return std::optional<Profile>
     */
    std::optional<Profile> ProfileStore::getActiveProfile() const
    {
        if (!_activeProfile.has())
            return std::nullopt;

        return getProfile(_activeProfile.get().value());
    }

    /**
     * @brief Get the name of the currently active profile
     *
     * @return std::optional<std::string>
     */
    std::optional<std::string> ProfileStore::getActiveProfileName() const
    {
        const auto profile = getActiveProfile();
        if (profile.has_value())
            return profile->getName();

        return std::nullopt;
    }

    /**
     * @brief Get a profile by its ID
     *
     * @note Ignores profiles marked as deleted
     *
     * @param id
     * @return std::optional<Profile>
     */
    std::optional<Profile> ProfileStore::getProfile(ProfileId id) const
    {
        auto profile = _get(HasProfileId(id) && !IsDeleted<Entry>());

        if (!profile.has_value())
            return std::nullopt;

        return profile;
    }

    /**
     * @brief Get a profile by its name
     *
     * @note Ignores profiles marked as deleted
     *
     * @param name
     * @return std::optional<Profile>
     */
    std::optional<Profile> ProfileStore::getProfile(std::string_view name) const
    {
        auto profile = _get(HasProfileName(name) && !IsDeleted<Entry>());

        if (profile.has_value())
            return profile;

        return std::nullopt;
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
        return getProfile(name).has_value();
    }

    /**
     * @brief Check if a profile with the same name as the given profile
     * exists
     *
     * @note Ignores profiles marked as deleted
     *
     * @param profile
     * @return true
     * @return false
     */
    bool ProfileStore::profileExists(const Profile& profile) const
    {
        return profileExists(profile.getName());
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
    ProfileStoreResult ProfileStore::addProfile(
        const drafts::ProfileDraft& draft
    )
    {
        _markPotentiallyDirty();

        auto* entry = _findEntry(HasProfileName(draft.name));

        if (entry != nullptr)
        {
            if (entry->state != StoreState::Deleted)
                return ProfileStoreResult::NameAlreadyExists;

            entry->state = StoreState::Modified;
            entry->value.setName(draft.name);
            entry->value.setEmail(draft.email);

            return ProfileStoreResult::Ok;
        }

        const auto newId = _generateNewId();
        _addEntry(Profile{newId, draft.name, draft.email}, StoreState::New);

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
    ProfileStoreResult ProfileStore::removeProfile(
        const drafts::ProfileDraft& draft
    )
    {
        _markPotentiallyDirty();

        auto* const entry = _findEntry(HasProfileName(draft.name));

        if (entry == nullptr)
            return ProfileStoreResult::ProfileNotFound;

        if (_activeProfile.has() &&
            entry->value.getId() == _activeProfile.get().value())
        {
            _activeProfile.unset();
        }

        // if the profile is new and not yet committed, just remove it
        // entirely
        if (entry->state == StoreState::New)
        {
            _removeEntry(entry->value.getId());
            return ProfileStoreResult::Ok;
        }

        entry->state = StoreState::Deleted;

        return ProfileStoreResult::Ok;
    }

    /**
     * @brief Commit a new profile to the underlying service
     *
     * @param entry
     */
    void ProfileStore::_commitNewProfile(Entry* entry)
    {
        const auto& name  = entry->value.getName();
        const auto& email = entry->value.getEmail();
        const auto  newId = _profileService->create(name, email);
        const auto  oldId = entry->value.getId();

        if (newId != oldId)
        {
            entry->value.setId(newId);
            entry->state = StoreState::Clean;

            // Update the active profile ID if it was pointing to the old ID
            if (_activeProfile.has() && _activeProfile.get().value() == oldId)
                _activeProfile.set(newId);
        }
        else
        {
            entry->state = StoreState::Clean;
        }

        LOG_INFO(
            std::format(
                "Profile '{}' saved to database",
                entry->value.getName()
            )
        );
    }

    /**
     * @brief Commit a modified profile to the underlying service
     *
     * @param entry
     */
    void ProfileStore::_commitModifiedProfile(Entry* entry)
    {
        const auto& name  = entry->value.getName();
        const auto& email = entry->value.getEmail();
        const auto  id    = entry->value.getId();

        _profileService->update(id, name, email);
        entry->state = StoreState::Clean;

        LOG_INFO(
            std::format(
                "Profile '{}' updated in database",
                entry->value.getName()
            )
        );
    }

    /**
     * @brief Commit a deleted profile to the underlying service
     *
     * @param entry
     */
    void ProfileStore::_commitDeletedProfile(Entry* entry)
    {
        const auto id = entry->value.getId();

        _profileService->remove(id);
        // NOTE: deleted entries will be removed in one go!

        LOG_INFO(
            std::format(
                "Profile '{}' removed from database",
                entry->value.getName()
            )
        );
    }

    /**
     * @brief Commit all pending changes to the underlying service
     *
     */
    void ProfileStore::commit()
    {
        if (!isDirty())
        {
            LOG_DEBUG("No changes to save in ProfileStore");
            return;
        }

        for (auto& entry : _getEntries())
        {
            switch (entry.state)
            {
                case StoreState::New:
                    _commitNewProfile(&entry);
                    break;
                case StoreState::Modified:
                    _commitModifiedProfile(&entry);
                    break;
                case StoreState::Deleted:
                    _commitDeletedProfile(&entry);
                    break;
                case StoreState::Clean:
                    break;
            }
        }

        _cleanEntries();
    }

    /**
     * @brief Subscribe to profile change events
     *
     * @param func
     * @param user
     * @return Connection
     */
    Connection ProfileStore::subscribeToProfileChange(
        OnProfileChanged::func func,
        void*                  user
    )
    {
        return _activeProfile.on<OnProfileChanged>(func, user);
    }

}   // namespace app