#include "store/profile_store.hpp"

#include <algorithm>
#include <cctype>
#include <format>
#include <unordered_map>

#include "drafts/profile_draft.hpp"
#include "services_api/i_profile_service.hpp"
#include "store/profile_store.hpp"

#define __LOG_CATEGORY__ LogCategory::app_store_ProfileStore
#include "logging/log_macros.hpp"

namespace app
{

    /**
     * @brief Construct a new Profile Store:: Profile Store object
     *
     * @note It loads all profiles from the underlying service upon
     * construction. Additionally, it initializes the internal state tracking
     * for each profile and maintains a set of used IDs to ensure uniqueness
     * when generating new profile IDs.
     *
     * @param profileService
     */
    ProfileStore::ProfileStore(IProfileService& profileService)
        : _profileService{profileService}
    {
        _profiles = _profileService.getAll();

        for (const auto& profile : _profiles)
        {
            _profileStates.emplace(profile.getId(), StoreState::Clean);
            _usedIds.emplace(profile.getId());
        }
    }

    /**
     * @brief Check if there are any profiles in the store
     *
     * @note This ignores profiles marked as deleted
     *
     * @return true
     * @return false
     */
    bool ProfileStore::hasProfiles() const
    {
        for (const auto& profile : _profiles)
            if (!_isDeleted(profile.getId()))
                return true;

        return false;
    }

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

        for (const auto& profile : _profiles)
            if (!_isDeleted(profile.getId()))
                names.push_back(profile.getName());

        return names;
    }

    /**
     * @brief Set the active profile by name
     *
     * @param name
     * @return ProfileStoreResult
     */
    ProfileStoreResult ProfileStore::setActiveProfile(
        std::optional<std::string_view> name
    )
    {
        if (!name.has_value())
        {
            _activeProfileId.reset();
            return ProfileStoreResult::Ok;
        }

        const auto profile = getProfile(name.value());
        if (!profile)
            return ProfileStoreResult::ProfileNotFound;

        _activeProfileId = profile->getId();

        return ProfileStoreResult::Ok;
    }

    /**
     * @brief Get the currently active profile
     *
     * @return std::optional<Profile>
     */
    std::optional<Profile> ProfileStore::getActiveProfile() const
    {
        if (!_activeProfileId)
            return std::nullopt;

        return getProfile(_activeProfileId.value());
    }

    /**
     * @brief Get the name of the currently active profile
     *
     * @return std::optional<std::string>
     */
    std::optional<std::string> ProfileStore::getActiveProfileName() const
    {
        if (const auto profile = getActiveProfile())
            return profile->getName();

        return std::nullopt;
    }

    /**
     * @brief Check if there are any pending changes in the store
     *
     * @return true
     * @return false
     */
    bool ProfileStore::hasPendingChanges() const
    {
        return std::ranges::any_of(
            _profileStates,
            [](const auto& pair) { return pair.second != StoreState::Clean; }
        );
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
        if (_isDeleted(id))
            return std::nullopt;

        return _findProfile(id);
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
        const auto profile = _findProfile(name);

        if (profile.has_value() && _isDeleted(profile->getId()))
            return std::nullopt;

        return profile;
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
     * @brief Add a new profile to the store
     *
     * @note If a profile with the same name exists and is marked as deleted,
     * it will be restored and updated with the new draft information.
     * Otherwise, a new profile will be created.
     *
     * @param draft
     * @return ProfileStoreResult
     */
    ProfileStoreResult ProfileStore::addProfile(
        const drafts::ProfileDraft& draft
    )
    {
        const auto profileOpt = _findProfile(draft.name);

        if (profileOpt.has_value() && !_isDeleted(profileOpt->getId()))
            return ProfileStoreResult::NameAlreadyExists;

        if (profileOpt.has_value() && _isDeleted(profileOpt->getId()))
        {
            const auto id      = profileOpt->getId();
            _profileStates[id] = StoreState::Modified;

            _updateInternal(id, draft.name, draft.email);

            return ProfileStoreResult::Ok;
        }

        const auto newId = _generateNewId();   // marks ID as used
        _profileStates.emplace(newId, StoreState::New);

        Profile newProfile{newId, draft.name, draft.email};
        _profiles.push_back(newProfile);
        return ProfileStoreResult::Ok;
    }

    /**
     * @brief Remove a profile from the store
     *
     * @note Marks the profile as deleted; actual removal occurs during commit
     *
     * @param draft
     * @return ProfileStoreResult
     */
    ProfileStoreResult ProfileStore::removeProfile(
        const drafts::ProfileDraft& draft
    )
    {
        const auto profile = _findProfile(draft.name);

        if (!profile.has_value())
            return ProfileStoreResult::ProfileNotFound;

        if (_activeProfileId.has_value() &&
            profile->getId() == _activeProfileId.value())
        {
            _activeProfileId.reset();
        }

        // if the profile is new and not yet committed, just remove it entirely
        if (_profileStates[profile->getId()] == StoreState::New)
        {
            _removeInternal(profile->getId());
            _profileStates.erase(profile->getId());
            _usedIds.erase(profile->getId());
            return ProfileStoreResult::Ok;
        }

        _profileStates[profile->getId()] = StoreState::Deleted;

        return ProfileStoreResult::Ok;
    }

    /**
     * @brief Reload all profiles from the underlying service
     *
     */
    void ProfileStore::reload()
    {
        _profiles = _profileService.getAll();

        _profileStates.clear();
        _usedIds.clear();

        for (const auto& profile : _profiles)
        {
            _profileStates.emplace(profile.getId(), StoreState::Clean);
            _usedIds.emplace(profile.getId());
        }

        if (_activeProfileId && !_findProfile(_activeProfileId.value()))
            _activeProfileId.reset();
    }

    /**
     * @brief Commit all pending changes to the underlying service
     *
     */
    void ProfileStore::commit()
    {
        if (!hasPendingChanges())
        {
            LOG_DEBUG("No changes to save in ProfileStore");
            return;
        }

        for (auto& p : _profiles)
        {
            if (_profileStates[p.getId()] == StoreState::New)
            {
                const auto name  = p.getName();
                const auto email = p.getEmail();
                const auto newId = _profileService.create(name, email);

                if (newId != p.getId())
                {
                    _profileStates.erase(p.getId());
                    _usedIds.erase(p.getId());
                    _profileStates.emplace(newId, StoreState::Clean);
                    _usedIds.insert(newId);
                    p.setId(newId);
                    if (_activeProfileId.has_value() &&
                        _activeProfileId.value() == p.getId())
                    {
                        _activeProfileId = newId;
                    }
                }
                else
                    _profileStates[p.getId()] = StoreState::Clean;

                LOG_INFO(
                    std::format("Profile '{}' saved to database", p.getName())
                );

                continue;
            }

            if (_profileStates[p.getId()] == StoreState::Modified)
            {
                const auto name  = p.getName();
                const auto email = p.getEmail();

                _profileService.update(p.getId(), name, email);
                _profileStates[p.getId()] = StoreState::Clean;

                LOG_INFO(
                    std::format("Profile '{}' updated in database", p.getName())
                );
                continue;
            }

            if (_profileStates[p.getId()] == StoreState::Deleted)
            {
                _profileService.remove(p.getId());
                _usedIds.erase(p.getId());
                _profileStates.erase(p.getId());

                LOG_INFO(
                    std::format(
                        "Profile '{}' removed from database",
                        p.getName()
                    )
                );
                continue;
            }
            _profileStates[p.getId()] = StoreState::Clean;
        }

        reload();
    }

    /**
     * @brief Normalize a profile name for comparison
     *
     * @note Trims whitespace and converts to lowercase for case-insensitive
     * comparison
     *
     * @param name
     * @return std::string
     */
    std::string ProfileStore::_normalizeName(std::string_view name)
    {
        // TODO: clean this up
        //  Policy: trim + case-insensitive
        //  If you want case-sensitive, remove the tolower part.
        std::size_t start = 0;
        while (start < name.size() &&
               std::isspace(static_cast<unsigned char>(name[start])))
            ++start;

        std::size_t end = name.size();
        while (end > start &&
               std::isspace(static_cast<unsigned char>(name[end - 1])))
            --end;

        std::string result;
        result.reserve(end - start);

        for (std::size_t i = start; i < end; ++i)
        {
            result.push_back(
                static_cast<char>(
                    std::tolower(static_cast<unsigned char>(name[i]))
                )
            );
        }

        return result;
    }

    /**
     * @brief generate a new unique ProfileId and mark it as used
     *
     * @return ProfileId
     */
    ProfileId ProfileStore::_generateNewId()
    {
        ProfileId newId{0};
        while (_usedIds.contains(newId) || _usedIds.contains(-newId))
            newId = ProfileId::from(newId.value() + 1);

        _usedIds.insert(newId);
        return newId;
    }

    /**
     * @brief Check if a profile with the given ID is marked as deleted
     *
     * @param id
     * @return true
     * @return false
     */
    bool ProfileStore::_isDeleted(ProfileId id) const
    {
        const auto it = _profileStates.find(id);
        if (it == _profileStates.end())
            return false;

        return it->second == StoreState::Deleted;
    }

    /**
     * @brief find a profile by its ID
     *
     * @param id
     * @return std::optional<Profile>
     */
    std::optional<Profile> ProfileStore::_findProfile(ProfileId id) const
    {
        const auto it = std::ranges::find_if(
            _profiles,
            [id](const Profile& p) { return p.getId() == id; }
        );

        if (it == _profiles.end())
            return std::nullopt;

        return *it;
    }

    /**
     * @brief find a profile by its name
     *
     * @param name
     * @return std::optional<Profile>
     */
    std::optional<Profile> ProfileStore::_findProfile(
        std::string_view name
    ) const
    {
        const std::string normalized = _normalizeName(name);

        const auto it = std::ranges::find_if(
            _profiles,
            [&normalized](const Profile& p)
            { return _normalizeName(p.getName()) == normalized; }
        );

        if (it == _profiles.end())
            return std::nullopt;

        return *it;
    }

    /**
     * @brief Remove a profile by its ID
     *
     * @param id
     */
    void ProfileStore::_removeInternal(ProfileId id)
    {
        _profiles.erase(
            std::remove_if(
                _profiles.begin(),
                _profiles.end(),
                [id](const Profile& p) { return p.getId() == id; }
            ),
            _profiles.end()
        );
    }

    /**
     * @brief Remove a profile by its name
     *
     * @param name
     */
    void ProfileStore::_removeInternal(std::string_view name)
    {
        const std::string normalized = _normalizeName(name);

        _profiles.erase(
            std::remove_if(
                _profiles.begin(),
                _profiles.end(),
                [&normalized, this](const Profile& p)
                { return _normalizeName(p.getName()) == normalized; }
            ),
            _profiles.end()
        );
    }

    /**
     * @brief Update a profile's name and email by its ID
     *
     * @param id
     * @param newName
     * @param newEmail
     */
    void ProfileStore::_updateInternal(
        ProfileId                  id,
        std::string_view           newName,
        std::optional<std::string> newEmail
    )
    {
        for (auto& profile : _profiles)
        {
            if (profile.getId() == id)
            {
                profile.setName(std::string{newName});
                profile.setEmail(newEmail);
                return;
            }
        }
    }

}   // namespace app