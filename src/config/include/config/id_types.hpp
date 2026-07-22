#ifndef __CONFIG__INCLUDE__CONFIG__ID_TYPES_HPP__
#define __CONFIG__INCLUDE__CONFIG__ID_TYPES_HPP__

#include "config/strong_id.hpp"

// clang-format off
struct ProfileTag {};
using ProfileId = StrongId<ProfileTag>;

struct SecurityTag {};
using SecurityId = StrongId<SecurityTag>;

struct InstrumentTag {};
using InstrumentId = StrongId<InstrumentTag>;

using InstrumentIdSeq = IdSequence<InstrumentId>;

template <typename T>
concept HasInstrumentId = requires(T type) { type.getInstrumentId(); };

struct StockTag {};
using StockId = StrongId<StockTag>;

struct OptionTag {};
using OptionId = StrongId<OptionTag>;

struct TransactionOptionTag {};
using TransactionOptionId = StrongId<TransactionOptionTag>;

struct AccountTag {};
using AccountId = StrongId<AccountTag>;

struct TransactionTag {};
using TransactionId = StrongId<TransactionTag>;

struct TransactionEntryTag {};
using TransactionEntryId = StrongId<TransactionEntryTag>;

struct TradeLegTag {};
using TradeLegId = StrongId<TradeLegTag>;

struct PositionTag {};
using PositionId = StrongId<PositionTag>;

struct WatchlistTag {};
using WatchlistId = StrongId<WatchlistTag>;

struct WatchlistInstrumentTag {};
using WatchlistInstrumentId = StrongId<WatchlistInstrumentTag>;

// clang-format on

template <typename T>
auto getId(const T& value);

#ifndef __CONFIG__INCLUDE__CONFIG__DETAILS__ID_TYPES_TPP__
#include "config/details/id_types.tpp"
#endif   // __CONFIG__INCLUDE__CONFIG__DETAILS__ID_TYPES_TPP__

#endif   // __CONFIG__INCLUDE__CONFIG__ID_TYPES_HPP__