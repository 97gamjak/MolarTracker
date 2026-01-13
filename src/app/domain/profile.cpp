#include "profile.hpp"

namespace app
{
    Profile::Profile(
        ProfileId                         id,
        const std::string&                name,
        const std::optional<std::string>& email
    )
        : _id{id}, _name{name}, _email{email}
    {
    }

}   // namespace app