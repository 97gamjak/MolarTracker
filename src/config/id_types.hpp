#ifndef __CONFIG__ID_TYPES_HPP__
#define __CONFIG__ID_TYPES_HPP__

#include "config/strong_id.hpp"

// clang-format off
struct ProfileTag {};
using ProfileId = StrongId<ProfileTag>;
// clang-format on

// clang-format off
struct CashTag {};
using CashId = StrongId<CashTag>;
// clang-format on

// clang-format off
struct AccountTag {};
using AccountId = StrongId<AccountTag>;
// clang-format on

// clang-format off
struct TransactionTag {};
using TransactionId = StrongId<TransactionTag>;
// clang-format on

#endif   // __CONFIG__ID_TYPES_HPP__