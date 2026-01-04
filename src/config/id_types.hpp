#ifndef __APP__CONFIG__ID_TYPES_HPP__
#define __APP__CONFIG__ID_TYPES_HPP__

#include "config/strong_id.hpp"

// clang-format off
struct ProfileTag {};
using ProfileId = StrongId<ProfileTag>;
// clang-format on

#endif   // __APP__CONFIG__ID_TYPES_HPP__