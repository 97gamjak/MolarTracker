#ifndef __APP__STORE__PROFILE__PROFILE_STORE_HPP__
#define __APP__STORE__PROFILE__PROFILE_STORE_HPP__

#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "app/domain/profile.hpp"
#include "app/store/base/base_store.hpp"
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
    class ProfileStore : public BaseStore<Profile, ProfileId>,
                         public Observable<OnProfileChanged>
    {
       private:
        /// reference to the profile service
        std::shared_ptr<IProfileService> _profileService;

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
        void _updateInternal(
            ProfileId                         id,
            std::string_view                  newName,
            const std::optional<std::string>& newEmail
        );

        void _commitNewProfile(Entry* entry);
        void _commitModifiedProfile(Entry* entry);
        void _commitDeletedProfile(Entry* entry);

        void _notifyProfileChanged(const std::optional<ProfileId>& profileId);
    };

}   // namespace app

#endif   // __APP__STORE__PROFILE__PROFILE_STORE_HPP__