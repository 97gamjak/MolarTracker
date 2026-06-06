#ifndef __STORE__SRC__STORE__PROFILE__PROFILE_STORE_HPP__
#define __STORE__SRC__STORE__PROFILE__PROFILE_STORE_HPP__

#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "active_profile.hpp"
#include "config/id_types.hpp"
#include "config/signal_tags.hpp"
#include "connections/observable.hpp"
#include "domain/profile.hpp"
#include "store/base/base_store.hpp"
#include "store/i_profile_store.hpp"

namespace service
{
    class IProfileService;   // forward declaration
}   // namespace service

namespace store
{
    /**
     * @brief Store for managing profiles
     *
     */
    class ProfileStore : public BaseStore<domain::Profile, ProfileId>,
                         public IProfileStore
    {
       private:
        /// reference to the profile service
        std::shared_ptr<service::IProfileService> _profileService;

        /// the ID of the currently active profile, this is used to determine
        /// which profile is currently active in the application and should be
        /// loaded when the application starts.
        ActiveProfile _activeProfile;

        /// alias for the base store type
        using Base = BaseStore<domain::Profile, ProfileId>;

       public:
        explicit ProfileStore(
            const std::shared_ptr<service::IProfileService>& profileService
        );

        [[nodiscard]] bool hasProfiles() const override;

        [[nodiscard]]
        std::vector<std::string> getAllProfileNames() const override;

        [[nodiscard]]
        ProfileStoreResult setActiveProfile(std::string_view name) override;

        [[nodiscard]]
        std::optional<domain::Profile> getActiveProfile() const override;

        [[nodiscard]] bool profileExists(std::string_view name) const override;

        [[nodiscard]]
        ProfileStoreResult addProfile(const domain::Profile& draft) override;

        [[nodiscard]]
        ProfileStoreResult removeProfile(const domain::Profile& draft) override;

        void commit() override;
        void reload() override;

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
        std::optional<domain::Profile> _getProfile(ProfileId id) const;
        [[nodiscard]]
        std::optional<domain::Profile> _getProfile(std::string_view) const;
    };

}   // namespace store

#endif   // __STORE__SRC__STORE__PROFILE__PROFILE_STORE_HPP__