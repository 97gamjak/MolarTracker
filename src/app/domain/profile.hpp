#ifndef __APP__DOMAIN__PROFILE_HPP__
#define __APP__DOMAIN__PROFILE_HPP__

#include <string>

#include "id_types.hpp"

class Profile
{
   private:
    ProfileID   _id;
    std::string _name;

   public:
    Profile(ProfileID id, const std::string& name);

    ProfileID          id() const noexcept;
    const std::string& name() const noexcept;
};

#endif   // __APP__DOMAIN__PROFILE_HPP__