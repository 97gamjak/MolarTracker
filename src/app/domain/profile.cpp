#include "profile.hpp"

namespace app
{
    Profile::Profile(ProfileID id, const std::string& name)
        : _id{id}, _name{name}
    {
    }

    ProfileID Profile::id() const noexcept { return _id; }

    std::string& Profile::name() const noexcept { return _name; }

    std::optional<std::string>& Profile::email() const noexcept
    {
        return _email;
    }

}   // namespace app