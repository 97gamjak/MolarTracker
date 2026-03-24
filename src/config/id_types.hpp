#ifndef __CONFIG__ID_TYPES_HPP__
#define __CONFIG__ID_TYPES_HPP__

#include "strong_id.hpp"

// clang-format off
struct ProfileTag {};
using ProfileId = StrongId<ProfileTag>;
// clang-format on

// clang-format off
struct SecurityTag {};
using SecurityId = StrongId<SecurityTag>;
// clang-format on

// clang-format off
struct InstrumentTag {};
using InstrumentId = StrongId<InstrumentTag>;
// clang-format on

// clang-format off
struct AccountTag {};
using AccountId = StrongId<AccountTag>;
// clang-format on

// clang-format off
struct TransactionTag {};
using TransactionId = StrongId<TransactionTag>;
// clang-format on

// clang-format off
struct TransactionEntryTag {};
using TransactionEntryId = StrongId<TransactionEntryTag>;
// clang-format on

template <typename T>
auto getId(const T& value);

#ifndef __CONFIG__ID_TYPES_TPP__
#include "id_types.tpp"
#endif   // __CONFIG__ID_TYPES_TPP__

#endif   // __CONFIG__ID_TYPES_HPP__