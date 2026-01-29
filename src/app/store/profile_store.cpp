#include "store/profile_store.hpp"

#include <algorithm>
#include <cctype>
#include <unordered_map>

#include "drafts/profile_draft.hpp"
#include "services_api/i_profile_service.hpp"
#include "store/profile_store.hpp"

#define __LOG_CATEGORY__ LogCategory::app_store_ProfileStore
#include "logging/log_macros.hpp"

namespace app
{

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

    bool ProfileStore::hasProfiles() const { return !_profiles.empty(); }

    std::vector<std::string> ProfileStore::getAllProfileNames() const
    {
        std::vector<std::string> names;
        names.reserve(_profiles.size());

        for (const auto& profile : _profiles)
            names.push_back(profile.getName());

        return names;
    }

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

    std::optional<Profile> ProfileStore::getActiveProfile() const
    {
        if (!_activeProfileId)
            return std::nullopt;

        return getProfile(_activeProfileId.value());
    }

    std::optional<std::string_view> ProfileStore::getActiveProfileName() const
    {
        if (const auto profile = getActiveProfile())
            return profile->getName();

        return std::nullopt;
    }

    bool ProfileStore::hasPendingChanges() const
    {
        return std::ranges::any_of(
            _profileStates,
            [](const auto& pair) { return pair.second != StoreState::Clean; }
        );
    }

    std::optional<Profile> ProfileStore::getProfile(ProfileId id) const
    {
        const auto it = std::ranges::find_if(
            _profiles,
            [id](const Profile& p) { return p.getId() == id; }
        );

        if (it == _profiles.end())
            return std::nullopt;

        return *it;
    }

    std::optional<Profile> ProfileStore::getProfile(std::string_view name) const
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

    bool ProfileStore::profileExists(std::string_view name) const
    {
        return getProfile(name).has_value();
    }

    ProfileStoreResult ProfileStore::addProfile(
        const drafts::ProfileDraft& draft
    )
    {
        if (profileExists(draft.name))
        {
            return ProfileStoreResult::NameAlreadyExists;
        }

        const ProfileId newId = _generateNewId();
        Profile         newProfile{newId, draft.name, draft.email};
        _profiles.push_back(newProfile);
        _profileStates.emplace(newId, StoreState::New);
        _usedIds.emplace(newId);
        return ProfileStoreResult::Ok;
    }

    ProfileStoreResult ProfileStore::removeProfile(
        const drafts::ProfileDraft& draft
    )
    {
        // TODO: centralize name normalization and finding logic
        const auto it = std::ranges::find_if(
            _profiles,
            [&draft](const Profile& p)
            {
                return _normalizeName(p.getName()) ==
                       _normalizeName(draft.name);
            }
        );

        if (it == _profiles.end())
            return ProfileStoreResult::ProfileNotFound;

        const ProfileId idToRemove = it->getId();
        _profileStates[idToRemove] = StoreState::Deleted;
        // Optionally remove from _profiles here or during commit
        return ProfileStoreResult::Ok;
    }

    void ProfileStore::reload() { _profiles = _profileService.getAll(); }

    void ProfileStore::commit()
    {
        if (!hasPendingChanges())
        {
            LOG_DEBUG("No changes to save in ProfileStore");
            return;
        }

        for (const auto& p : _profiles)
        {
            if (_profileStates[p.getId()] == StoreState::New)
            {
                // TODO: handle ID assignment properly
                const auto newId =
                    _profileService.create(p.getName(), p.getEmail());
                if (newId != p.getId())
                {
                    // TODO: handle this is a centralized manner
                    _profileStates.erase(p.getId());
                    _usedIds.erase(p.getId());
                    _profileStates.emplace(newId, StoreState::Clean);
                    _usedIds.insert(newId);
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
                // TODO: handle ID assignment properly
                // _profileService.update(p);
                _profileStates[p.getId()] = StoreState::Clean;
                continue;
            }

            if (_profileStates[p.getId()] == StoreState::Deleted)
            {
                _profileService.remove(p.getId());
                _usedIds.erase(p.getId());
                _profileStates.erase(p.getId());
                _profiles.erase(
                    std::remove_if(
                        _profiles.begin(),
                        _profiles.end(),
                        [&p](const Profile& profile)
                        { return profile.getId() == p.getId(); }
                    ),
                    _profiles.end()
                );

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

    std::string ProfileStore::_normalizeName(std::string_view name)
    {
        // Policy: trim + case-insensitive
        // If you want case-sensitive, remove the tolower part.
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

    ProfileId ProfileStore::_generateNewId()
    {
        ProfileId newId{0};
        while (_usedIds.contains(newId) || _usedIds.contains(-newId))
        {
            newId = ProfileId::from(newId.value() + 1);
        }
        _usedIds.insert(newId);
        return newId;
    }

}   // namespace app