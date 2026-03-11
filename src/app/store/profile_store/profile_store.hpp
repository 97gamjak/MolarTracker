#ifndef __APP__STORE__PROFILE_STORE__PROFILE_STORE_HPP__
#define __APP__STORE__PROFILE_STORE__PROFILE_STORE_HPP__

#include <memory>
#include <optional>
#include <set>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "app/domain/profile.hpp"
#include "app/store/i_store.hpp"
#include "config/id_types.hpp"
#include "config/signal_tags.hpp"
#include "connections/observable.hpp"

namespace drafts
{
    struct ProfileDraft;   // forward declaration
}   // namespace drafts

namespace app
{

    class IProfileService;   // forward declaration

    /**
     * @brief Result of profile store operations
     *
     */
    enum class ProfileStoreResult : std::uint8_t
    {
        Ok,
        Error,
        NameAlreadyExists,
        ProfileNotFound,
    };

    /**
     * @brief Store for managing profiles
     *
     */
    // TODO(97gamjak): consider refactoring to avoid multiple inheritance
    // NOLINTNEXTLINE(fuchsia-multiple-inheritance)
    class ProfileStore : public IStore, public Observable<OnProfileChanged>
    {
       private:
        /// reference to the profile service
        std::shared_ptr<IProfileService> _profileService;

        /// in-memory cache of profiles, this is the source of truth for the UI
        /// and is only committed to the profile service when commit() is
        /// called.
        std::vector<Profile> _profiles;

        /// set of used profile IDs to ensure uniqueness when generating new IDs
        std::set<ProfileId> _usedIds;

        /// map of profile IDs to their current state in the store (clean, new,
        /// modified, deleted), this is used to track changes to the profiles
        /// and determine what needs to be committed to the profile service when
        /// commit() is called.
        std::unordered_map<ProfileId, StoreState, ProfileId::Hash>
            _profileStates;

        /// the ID of the currently active profile, this is used to determine
        /// which profile is currently active in the application and should be
        /// loaded when the application starts.
        std::optional<ProfileId> _activeProfileId;

       public:
        explicit ProfileStore(
            const std::shared_ptr<IProfileService>& profileService
        );

        void reload();

        [[nodiscard]] bool hasProfiles() const;

        [[nodiscard]] std::vector<std::string> getAllProfileNames() const;

        void setActiveProfile(std::optional<std::string_view> name);
        void unsetActiveProfile();
        [[nodiscard]] bool                       hasActiveProfile() const;
        [[nodiscard]] std::optional<Profile>     getActiveProfile() const;
        [[nodiscard]] std::optional<std::string> getActiveProfileName() const;

        [[nodiscard]] bool isDirty() const override;

        [[nodiscard]] std::optional<Profile> getProfile(ProfileId id) const;
        [[nodiscard]] std::optional<Profile> getProfile(std::string_view) const;

        // cppcheck-suppress functionStatic -- false positive
        [[nodiscard]] bool profileExists(const Profile& profile) const;
        [[nodiscard]] bool profileExists(std::string_view name) const;

        [[nodiscard]] ProfileStoreResult addProfile(
            const drafts::ProfileDraft& draft
        );

        [[nodiscard]] ProfileStoreResult removeProfile(
            const drafts::ProfileDraft& draft
        );

        void commit() override;

        Connection subscribeToProfileChange(
            OnProfileChanged::func func,
            void*                  user
        );

       private:
        [[nodiscard]] ProfileId _generateNewId();
        [[nodiscard]] bool      _isDeleted(ProfileId id) const;

        [[nodiscard]] std::optional<Profile> _findProfile(ProfileId id) const;
        [[nodiscard]] std::optional<Profile> _findProfile(
            std::string_view name
        ) const;

        void _removeInternal(ProfileId id);
        void _removeInternal(std::string_view name);

        void _updateInternal(
            ProfileId                         id,
            std::string_view                  newName,
            const std::optional<std::string>& newEmail
        );

        void _commitNewProfile(const Profile& profile);
        void _commitModifiedProfile(const Profile& profile);
        void _commitDeletedProfile(const Profile& profile);

        void _notifyProfileChanged(const std::optional<ProfileId>& profileId);
    };

}   // namespace app

#endif   // __APP__STORE__PROFILE_STORE__PROFILE_STORE_HPP__