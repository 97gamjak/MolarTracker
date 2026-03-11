#include <format>

#include "profile_store.hpp"

namespace app
{
    /**
     * @brief Set the active profile by name
     *
     * @param name
     */
    void ProfileStore::setActiveProfile(std::optional<std::string_view> name)
    {
        if (!name.has_value())
        {
            _activeProfileId.reset();
            return;
        }

        const auto profile = getProfile(name.value());
        if (!profile)
        {
            // TODO(97gamjak): make here MT specific error handling for stores
            // https://97gamjak.atlassian.net/browse/MOLTRACK-88
            throw std::runtime_error(
                std::format("Profile '{}' not found", name.value())
            );
        }

        _activeProfileId = profile->getId();
        _notifyProfileChanged(_activeProfileId);
    }

    /**
     * @brief Unset the active profile, leaving no profile active
     *
     */
    void ProfileStore::unsetActiveProfile()
    {
        _activeProfileId.reset();
        _notifyProfileChanged(std::nullopt);
    }

}   // namespace app