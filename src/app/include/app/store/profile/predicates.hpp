#ifndef __APP__INCLUDE__APP__STORE__PROFILE__PREDICATES_HPP__
#define __APP__INCLUDE__APP__STORE__PROFILE__PREDICATES_HPP__

#include "app/domain/profile.hpp"
#include "app/store/base/predicate.hpp"

namespace app
{

    Predicate<Profile> HasProfileId(ProfileId id);

    Predicate<Profile> HasProfileName(std::string_view name);

}   // namespace app

#endif   // __APP__INCLUDE__APP__STORE__PROFILE__PREDICATES_HPP__