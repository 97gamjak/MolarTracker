#include <format>

#include "profile_store.hpp"

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
     */
    void ActiveProfile::set(const std::optional<ProfileId>& profileId)
    {
        _value = profileId;
        _emit<OnProfileChanged>(_value);
    }

    /**
     * @brief Unset the active profile, leaving no profile active
     *
     */
    void ActiveProfile::unset()
    {
        _value.reset();
        _emit<OnProfileChanged>(std::nullopt);
    }
}   // namespace app