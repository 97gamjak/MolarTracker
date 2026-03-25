#ifndef __APP__INCLUDE__APP__STORE__PROFILE__PREDICATES_HPP__
#define __APP__INCLUDE__APP__STORE__PROFILE__PREDICATES_HPP__

#include "app/domain/profile.hpp"
#include "app/store/base/base_store.hpp"
#include "app/store/base/predicate.hpp"

namespace app
{

    Predicate<BaseStore<Profile, ProfileId>::Entry> HasProfileId(ProfileId id);

    Predicate<BaseStore<Profile, ProfileId>::Entry> HasProfileName(
        std::string_view name
    );

}   // namespace app

#endif   // __APP__INCLUDE__APP__STORE__PROFILE__PREDICATES_HPP__