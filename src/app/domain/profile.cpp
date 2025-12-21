#include "profile.hpp"

Profile::Profile(ProfileID id, const std::string& name) : _id{id}, _name{name}
{
}

ProfileID Profile::id() const noexcept { return _id; }

const std::string& Profile::name() const noexcept { return _name; }