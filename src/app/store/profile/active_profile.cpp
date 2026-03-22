#include "active_profile.hpp"

#include <optional>

#include "config/signal_tags.hpp"

namespace app
{
    /**
     * @brief Get the active profile ID
     *
     * @return std::optional<ProfileId>
     */
    std::optional<ProfileId> ActiveProfile::get() const { return _value; }

    /**
     * @brief Check if there is an active profile
     *
     * @return true
     * @return false
     */
    bool ActiveProfile::has() const { return _value.has_value(); }

    /**
     * @brief Set the active profile by ID
     *
     * @param profileId
     * @param reason The reason for the profile change, this can be used by
     * subscribers to the OnProfileChanged signal to determine how to respond to
     * the change (e.g., if the profile was changed due to a user action, they
     * might want to refresh the UI, if it was changed due to a database
     * operation, they might want to update cached data, etc.)
     */
    void ActiveProfile::set(
        const std::optional<ProfileId>& profileId,
        OnProfileChanged::Reason        reason
    )
    {
        _value = profileId;
        _emit<OnProfileChanged>(_value, reason);
    }

    /**
     * @brief Unset the active profile, leaving no profile active
     *
     */
    void ActiveProfile::unset()
    {
        _value.reset();
        _emit<OnProfileChanged>(
            std::nullopt,
            OnProfileChanged::Reason::UnsetProfile
        );
    }
}   // namespace app