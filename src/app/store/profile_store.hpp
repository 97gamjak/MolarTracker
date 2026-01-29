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

#include "app/domain/profile.hpp"
#include "app/services_api/i_profile_service.hpp"
#include "app/store/i_store.hpp"
#include "app/store/subscription.hpp"

namespace drafts
{
    struct ProfileDraft;
}

namespace app
{
    // TODO: centralize this enum
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
    class ProfileStore : public IStore
    {
       private:
        IProfileService& _profileService;

        std::vector<Profile> _profiles;
        std::set<ProfileId>  _usedIds;
        std::unordered_map<ProfileId, StoreState, ProfileId::Hash>
            _profileStates;

        std::optional<ProfileId> _activeProfileId;
        // std::size_t                               _nextObserverId = 0;
        // std::unordered_map<std::size_t, Observer> _observers;

       public:
        explicit ProfileStore(IProfileService& profileService);

        void reload();

        [[nodiscard]] bool hasProfiles() const;

        [[nodiscard]] std::vector<std::string> getAllProfileNames() const;

        [[nodiscard]] ProfileStoreResult setActiveProfile(
            std::optional<std::string_view> name
        );
        [[nodiscard]] std::optional<Profile>     getActiveProfile() const;
        [[nodiscard]] std::optional<std::string> getActiveProfileName() const;

        [[nodiscard]] bool hasPendingChanges() const;

        [[nodiscard]] std::optional<Profile> getProfile(ProfileId id) const;
        [[nodiscard]] std::optional<Profile> getProfile(std::string_view) const;

        [[nodiscard]] bool profileExists(std::string_view name) const;

        [[nodiscard]] ProfileStoreResult addProfile(
            const drafts::ProfileDraft& draft
        );

        [[nodiscard]] ProfileStoreResult removeProfile(
            const drafts::ProfileDraft& draft
        );

        void commit() override;

       private:
        [[nodiscard]] static std::string _normalizeName(std::string_view name);
        [[nodiscard]] ProfileId          _generateNewId();
        [[nodiscard]] bool               _isDeleted(ProfileId id) const;

        [[nodiscard]] std::optional<Profile> _findProfile(ProfileId id) const;
        [[nodiscard]] std::optional<Profile> _findProfile(
            std::string_view name
        ) const;

        void _removeInternal(ProfileId id);
        void _removeInternal(std::string_view name);

        void _updateInternal(
            ProfileId                  id,
            std::string_view           newName,
            std::optional<std::string> newEmail
        );
    };

}   // namespace app

#endif   // __APP__STORE__PROFILE_STORE_HPP__