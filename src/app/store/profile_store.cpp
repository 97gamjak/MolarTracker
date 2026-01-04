#include "store/profile_store.hpp"

#include <algorithm>
#include <cctype>
#include <unordered_map>

#include "services_api/i_profile_service.hpp"
#include "store/profile_store.hpp"

namespace app
{

    ProfileStore::ProfileStore(IProfileService& profileService)
        : _profileService{profileService}
    {
    }

    const std::vector<Profile>& ProfileStore::profiles() const noexcept
    {
        return _profiles;
    }

    std::optional<ProfileId> ProfileStore::activeProfileId() const noexcept
    {
        return _activeProfileId;
    }

    const Profile* ProfileStore::activeProfile() const noexcept
    {
        if (!_activeProfileId)
            return nullptr;
        return findProfileById(*_activeProfileId);
    }

    bool ProfileStore::hasPendingChanges() const noexcept
    {
        return _hasPendingChanges;
    }

    const Profile* ProfileStore::findProfileById(ProfileId id) const noexcept
    {
        const auto it = std::find_if(
            _profiles.begin(),
            _profiles.end(),
            [id](const Profile& p) { return p.getId() == id; }
        );
        return it == _profiles.end() ? nullptr : &(*it);
    }

    const Profile* ProfileStore::findProfileByName(
        std::string_view name
    ) const noexcept
    {
        const std::string normalized = normalizeName(name);

        const auto it = std::find_if(
            _profiles.begin(),
            _profiles.end(),
            [&](const Profile& p)
            { return normalizeName(p.getName()) == normalized; }
        );
        return it == _profiles.end() ? nullptr : &(*it);
    }

    bool ProfileStore::profileNameExists(std::string_view name) const noexcept
    {
        return findProfileByName(name) != nullptr;
    }

    bool ProfileStore::profileNameExists(
        std::string_view name,
        ProfileId        ignoreProfileId
    ) const noexcept
    {
        const std::string normalized = normalizeName(name);

        for (const auto& profile : _profiles)
        {
            if (profile.getId() == ignoreProfileId)
                continue;
            if (normalizeName(profile.getName()) == normalized)
                return true;
        }
        return false;
    }

    Subscription ProfileStore::subscribe(Observer observer)
    {
        const std::size_t id = ++_nextObserverId;
        _observers.emplace(id, std::move(observer));
        return Subscription{
            [this](std::size_t observerId) { _observers.erase(observerId); },
            id
        };
    }

    void ProfileStore::reload()
    {
        _baselineProfiles = _profileService.getAllProfiles();
        _profiles         = _baselineProfiles;

        _hasPendingChanges = false;

        if (_profiles.empty())
        {
            _activeProfileId.reset();
        }
        else if (!_activeProfileId || !findProfileById(*_activeProfileId))
        {
            _activeProfileId = _profiles.front().getId();
        }

        notify();
    }

    void ProfileStore::discard()
    {
        _profiles          = _baselineProfiles;
        _hasPendingChanges = false;

        chooseFallbackActiveIfInvalid();
        notify();
    }

    ProfileId ProfileStore::createProfile(
        std::string                name,
        std::optional<std::string> email
    )
    {
        // In-memory only: we assign a temporary negative id policy later if you
        // want. For now keep it simple: create with id=0 means “not persisted
        // yet”. If you already have a temp-id scheme, tell me and I’ll match
        // it.
        Profile profile{ProfileId::from(0), name, email};

        _profiles.push_back(profile);

        if (!_activeProfileId)
            _activeProfileId = profile.getId();

        _hasPendingChanges = true;
        notify();
        return profile.getId();
    }

    void ProfileStore::renameProfile(ProfileId id, std::string newName)
    {
        auto it = std::find_if(
            _profiles.begin(),
            _profiles.end(),
            [id](const Profile& p) { return p.getId() == id; }
        );
        if (it == _profiles.end())
            return;

        if (it->getName() == newName)
            return;

        it->setName(std::move(newName));
        _hasPendingChanges = true;
        notify();
    }

    void ProfileStore::setActiveProfile(ProfileId id)
    {
        if (_activeProfileId && *_activeProfileId == id)
            return;

        if (!findProfileById(id))
            return;

        _activeProfileId = id;
        notify();
    }

    void ProfileStore::deleteProfile(ProfileId id)
    {
        const auto oldSize = _profiles.size();

        _profiles.erase(
            std::remove_if(
                _profiles.begin(),
                _profiles.end(),
                [id](const Profile& p) { return p.getId() == id; }
            ),
            _profiles.end()
        );

        if (_profiles.size() == oldSize)
            return;

        _hasPendingChanges = true;

        if (_activeProfileId && *_activeProfileId == id)
            chooseFallbackActiveIfInvalid();

        notify();
    }

    void ProfileStore::commit()
    {
        if (!_hasPendingChanges)
            return;

        // Diff baseline -> current and persist final state.
        // Minimal approach:
        // - detect deletes (in baseline but not in current)
        // - detect creates (in current but not in baseline)
        // - detect updates (same id but different fields)

        std::unordered_map<std::int64_t, Profile> baselineById;
        baselineById.reserve(_baselineProfiles.size());
        for (const auto& p : _baselineProfiles)
            baselineById.emplace(p.getId().value(), p);

        std::unordered_map<std::int64_t, Profile> currentById;
        currentById.reserve(_profiles.size());
        for (const auto& p : _profiles)
            currentById.emplace(p.getId().value(), p);

        // Deletes
        for (const auto& p : _baselineProfiles)
        {
            if (!currentById.contains(p.getId().value()))
            {
                _profileService.deleteProfile(p.getId());
            }
        }

        // Creates / Updates
        for (const auto& p : _profiles)
        {
            const bool existedBefore = baselineById.contains(p.getId().value());

            if (!existedBefore)
            {
                // If you use id=0 for new, service returns real id.
                const ProfileId newId =
                    _profileService.createProfile(p.getName(), p.getEmail());
                (void) newId;   // if you want, we can remap in-memory ids and
                                // keep active stable
                continue;
            }

            const Profile& old = baselineById.at(p.getId().value());
            if (old.getName() != p.getName() || old.getEmail() != p.getEmail())
            {
                // TODO:
                // _profileService.updateProfile(
                //     p
                // );   // or rename/update depending on your service API
            }
        }

        reload();
    }

    void ProfileStore::notify()
    {
        const auto copy = _observers;
        for (const auto& [_, observer] : copy)
        {
            if (observer)
                observer();
        }
    }

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

    void ProfileStore::chooseFallbackActiveIfInvalid()
    {
        if (_activeProfileId && findProfileById(*_activeProfileId))
            return;

        if (_profiles.empty())
            _activeProfileId.reset();
        else
            _activeProfileId = _profiles.front().getId();
    }

}   // namespace app