#ifndef __APP__STORE__PROFILE__ACTIVE_PROFILE_HPP__
#define __APP__STORE__PROFILE__ACTIVE_PROFILE_HPP__

#include "config/id_types.hpp"
#include "config/signal_tags.hpp"
#include "connections/observable.hpp"

namespace app
{
    /**
     * @brief Class for managing the active profile in the application, this
     * class is responsible for keeping track of which profile is currently
     * active in the application, allowing other parts of the application to
     * subscribe to changes in the active profile, and providing methods for
     * setting and unsetting the active profile.
     *
     */
    class ActiveProfile : public Observable<OnProfileChanged>
    {
       private:
        /// the ID of the currently active profile, this is used to determine
        /// which profile is currently active in the application and should be
        /// loaded when the application starts.
        std::optional<ProfileId> _value = std::nullopt;

       public:
        [[nodiscard]] std::optional<ProfileId> get() const;
        [[nodiscard]] bool                     has() const;

        void set(const std::optional<ProfileId>& profileId);
        void unset();
    };

}   // namespace app

#endif   // __APP__STORE__PROFILE__ACTIVE_PROFILE_HPP__