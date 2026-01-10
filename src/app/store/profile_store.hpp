#ifndef __APP__STORE__PROFILE_STORE_HPP__
#define __APP__STORE__PROFILE_STORE_HPP__

#include <cstddef>
#include <functional>
#include <optional>
#include <set>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "domain/profile.hpp"
#include "services_api/i_profile_service.hpp"
#include "store/subscription.hpp"

namespace drafts
{
    struct ProfileDraft;
}

namespace app
{
    enum class StoreState
    {
        Clean,
        New,
        Modified,
        Deleted
    };

    enum class ProfileStoreResult
    {
        Ok,
        Error,
        NameAlreadyExists,
        ProfileNotFound,
    };

    // TODO: add nodiscard where appropriate
    class ProfileStore final
    {
       private:
        IProfileService& _profileService;

        std::vector<Profile> _profiles;
        std::unordered_map<ProfileId, StoreState, ProfileId::Hash>
                            _profileStates;
        std::set<ProfileId> _usedIds;

        std::optional<ProfileId> _activeProfileId;
        // std::size_t                               _nextObserverId = 0;
        // std::unordered_map<std::size_t, Observer> _observers;

       public:
        explicit ProfileStore(IProfileService& profileService);

        bool hasProfiles() const;

        std::vector<std::string> getAllProfileNames() const;

        ProfileStoreResult     setActiveProfile(std::string_view name) noexcept;
        std::optional<Profile> getActiveProfile() const noexcept;

        bool hasPendingChanges() const noexcept;

        std::optional<Profile> getProfile(ProfileId id) const noexcept;
        std::optional<Profile> getProfile(std::string_view name) const noexcept;

        bool profileExists(std::string_view name) const noexcept;

        ProfileStoreResult addProfile(
            const drafts::ProfileDraft& draft
        ) noexcept;

        // // ----- observation -----
        // using Observer = std::function<void()>;
        // Subscription subscribe(Observer observer);

        // // ----- lifecycle -----
        // void reload();    // load from DB into store
        // void discard();   // drop in-memory changes -> back to last reload()

        // // ----- mutations (in-memory only) -----
        // ProfileId createProfile(
        //     std::string                name,
        //     std::optional<std::string> email = std::nullopt
        // );
        // void renameProfile(ProfileId id, std::string newName);
        // void setActiveProfile(ProfileId id);
        // void deleteProfile(ProfileId id);

        // // ----- persistence -----
        // void commit();   // apply changes to DB via service; then reload()

       private:
        // void notify();

        static std::string normalizeName(std::string_view name);

        ProfileId _generateNewId();

        // void chooseFallbackActiveIfInvalid();
    };

}   // namespace app

#endif   // __APP__STORE__PROFILE_STORE_HPP__