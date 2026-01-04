#ifndef __APP__STORE__PROFILE_STORE_HPP__
#define __APP__STORE__PROFILE_STORE_HPP__

#include <cstddef>
#include <functional>
#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "domain/profile.hpp"
#include "services_api/i_profile_service.hpp"
#include "store/subscription.hpp"

namespace app
{

    class ProfileStore final
    {
       public:
        explicit ProfileStore(IProfileService& profileService);

        // ----- state access -----
        const std::vector<Profile>& profiles() const noexcept;
        std::optional<ProfileId>    activeProfileId() const noexcept;
        const Profile*              activeProfile() const noexcept;

        bool hasPendingChanges() const noexcept;

        // ----- lookup / validation helpers -----
        const Profile* findProfileById(ProfileId id) const noexcept;
        const Profile* findProfileByName(std::string_view name) const noexcept;

        bool profileNameExists(std::string_view name) const noexcept;
        bool profileNameExists(
            std::string_view name,
            ProfileId        ignoreProfileId
        ) const noexcept;

        // ----- observation -----
        using Observer = std::function<void()>;
        Subscription subscribe(Observer observer);

        // ----- lifecycle -----
        void reload();    // load from DB into store
        void discard();   // drop in-memory changes -> back to last reload()

        // ----- mutations (in-memory only) -----
        ProfileId createProfile(
            std::string                name,
            std::optional<std::string> email = std::nullopt
        );
        void renameProfile(ProfileId id, std::string newName);
        void setActiveProfile(ProfileId id);
        void deleteProfile(ProfileId id);

        // ----- persistence -----
        void commit();   // apply changes to DB via service; then reload()

       private:
        void notify();

        static std::string normalizeName(
            std::string_view name
        );   // define policy here (trim + maybe lower)

        void chooseFallbackActiveIfInvalid();

       private:
        IProfileService& _profileService;

        // last loaded committed snapshot
        std::vector<Profile> _baselineProfiles;

        // current in-memory truth
        std::vector<Profile> _profiles;

        std::optional<ProfileId> _activeProfileId;

        // observation
        std::size_t                               _nextObserverId = 0;
        std::unordered_map<std::size_t, Observer> _observers;

        // dirty state
        bool _hasPendingChanges = false;
    };

}   // namespace app

#endif   // __APP__STORE__PROFILE_STORE_HPP__