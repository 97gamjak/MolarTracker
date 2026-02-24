#ifndef __CONFIG__ID_TYPES_HPP__
#define __CONFIG__ID_TYPES_HPP__

#include "config/strong_id.hpp"
#include "mstd/enum.hpp"

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

#define INSTRUMENT_KIND_LIST(X) \
    X(Security)                 \
    X(Cash)

MSTD_ENUM(InstrumentKind, std::uint8_t, INSTRUMENT_KIND_LIST);

#endif   // __CONFIG__ID_TYPES_HPP__