#include "profile.hpp"

namespace app
{
    /**
     * @brief Construct a new Profile:: Profile object
     *
     * @param id
     * @param name
     * @param email
     */
    Profile::Profile(
        ProfileId                         id,
        const std::string&                name,
        const std::optional<std::string>& email
    )
        : _id{id}, _name{name}, _email{email}
    {
    }

}   // namespace app