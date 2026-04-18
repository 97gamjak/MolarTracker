#ifndef __APP__INCLUDE__APP__STORE__PROFILE__ACTIVE_PROFILE_HPP__
#define __APP__INCLUDE__APP__STORE__PROFILE__ACTIVE_PROFILE_HPP__

#include <optional>

#include "config/id_types.hpp"
#include "config/signal_tags.hpp"
#include "connections/observable.hpp"

namespace app
{

    /**
     * @brief ActiveProfile manages the currently active profile in the
     * application.
     *
     * It allows setting and unsetting the active profile, as well as
     * subscribing to changes in the active profile through the OnProfileChanged
     * signal.
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

#endif   // __APP__INCLUDE__APP__STORE__PROFILE__ACTIVE_PROFILE_HPP__