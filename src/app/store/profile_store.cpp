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

    ProfileStoreResult ProfileStore::setActiveProfile(std::string_view name)
    {
        const auto profile = getProfile(name);
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
        const std::string normalized = normalizeName(name);

        const auto it = std::ranges::find_if(
            _profiles,
            [&normalized](const Profile& p)
            { return normalizeName(p.getName()) == normalized; }
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

    // Subscription ProfileStore::subscribe(Observer observer)
    // {
    //     const std::size_t id = ++_nextObserverId;
    //     _observers.emplace(id, std::move(observer));
    //     return Subscription{
    //         [this](std::size_t observerId) { _observers.erase(observerId); },
    //         id
    //     };
    // }

    // void ProfileStore::reload()
    // {
    //     _profiles = _profileService.getAllProfiles();

    //     notify();
    // }

    void ProfileStore::commit()
    {
        if (!hasPendingChanges())
        {
            LOG_DEBUG("No changes to commit in ProfileStore");
            return;
        }

        // Minimal approach:
        // - detect deletes (in baseline but not in current)
        // - detect creates (in current but not in baseline)
        // - detect updates (same id but different fields)

        for (const auto& p : _profiles)
        {
            if (_profileStates[p.getId()] == StoreState::New)
            {
                // TODO: handle ID assignment properly
                _profileService.create(p.getName(), p.getEmail());
                _profileStates[p.getId()] = StoreState::Clean;
                continue;
                ;
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
                // Optionally remove from _profiles here
                continue;
            }
            _profileStates[p.getId()] = StoreState::Clean;
        }

        // TODO: handle deletions properly
        // reload();
    }

    // void ProfileStore::notify()
    // {
    //     const auto copy = _observers;
    //     for (const auto& [_, observer] : copy)
    //     {
    //         if (observer)
    //             observer();
    //     }
    // }

    std::string ProfileStore::normalizeName(std::string_view name)
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

    // void ProfileStore::chooseFallbackActiveIfInvalid()
    // {
    //     if (_activeProfileId && findProfileById(*_activeProfileId))
    //         return;

    //     if (_profiles.empty())
    //         _activeProfileId.reset();
    //     else
    //         _activeProfileId = _profiles.front().getId();
    // }

}   // namespace app