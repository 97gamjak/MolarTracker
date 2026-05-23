#ifndef __APP__SRC__APP__STORE__PROFILE__PROFILE_STORE_HPP__
#define __APP__SRC__APP__STORE__PROFILE__PROFILE_STORE_HPP__

#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "active_profile.hpp"
#include "app/store/base/base_store.hpp"
#include "app/store/i_profile_store.hpp"
#include "config/id_types.hpp"
#include "config/signal_tags.hpp"
#include "connections/observable.hpp"
#include "domain/profile.hpp"

namespace drafts
{
    class ProfileDraft;   // forward declaration
}   // namespace drafts

namespace app
{

    class IProfileService;   // forward declaration

    /**
     * @brief Store for managing profiles
     *
     */
    class ProfileStore : public BaseStore<domain::Profile, ProfileId>,
                         public IProfileStore
    {
       private:
        /// reference to the profile service
        std::shared_ptr<IProfileService> _profileService;

        /// the ID of the currently active profile, this is used to determine
        /// which profile is currently active in the application and should be
        /// loaded when the application starts.
        ActiveProfile _activeProfile;

        /// alias for the base store type
        using Base = BaseStore<domain::Profile, ProfileId>;

       public:
        explicit ProfileStore(
            const std::shared_ptr<IProfileService>& profileService
        );

        [[nodiscard]] bool hasProfiles() const override;

        [[nodiscard]]
        std::vector<std::string> getAllProfileNames() const override;

        void setActiveProfile(std::string_view name) override;

        [[nodiscard]]
        std::optional<drafts::ProfileDraft> getActiveProfile() const override;

        [[nodiscard]] bool profileExists(std::string_view name) const override;

        [[nodiscard]]
        ProfileStoreResult addProfile(
            const drafts::ProfileDraft& draft
        ) override;

        [[nodiscard]]
        ProfileStoreResult removeProfile(
            const drafts::ProfileDraft& draft
        ) override;

        void commit() override;

        [[nodiscard]]
        Connection subscribeToProfileChange(
            const OnProfileChanged::func& func,
            void*                         user
        ) override;

       private:
        void _commitNewProfile(Entry entry);
        void _commitModifiedProfile(const Entry& entry);
        void _commitDeletedProfile(const Entry& entry);

        [[nodiscard]]
        std::optional<drafts::ProfileDraft> _getProfile(ProfileId id) const;
        [[nodiscard]]
        std::optional<drafts::ProfileDraft> _getProfile(std::string_view) const;
    };

}   // namespace app

#endif   // __APP__SRC__APP__STORE__PROFILE__PROFILE_STORE_HPP__