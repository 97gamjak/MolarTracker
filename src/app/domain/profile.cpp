#include "profile.hpp"

namespace app
{
    Profile::Profile(
        ProfileID                         id,
        const std::string&                name,
        const std::optional<std::string>& email
    )
        : _id{id}, _name{name}, _email{email}
    {
    }

    ProfileID Profile::id() const noexcept { return _id; }

    const std::string Profile::name() const noexcept { return _name; }

    const std::optional<std::string> Profile::email() const noexcept
    {
        return _email;
    }

}   // namespace app