#ifndef __CONFIG__INCLUDE__CONFIG__FINANCE_HPP__
#define __CONFIG__INCLUDE__CONFIG__FINANCE_HPP__

#include <cstdint>
#include <mstd/enum.hpp>

// NOLINTBEGIN(cppcoreguidelines-macro-usage)

#define CURRENCY_LIST(X) \
    X(USD)               \
    X(EUR)               \
    X(GBP)               \
    X(CHF)

MSTD_ENUM(Currency, std::uint8_t, CURRENCY_LIST);

#define ACCOUNT_KIND_LIST(X) \
    X(Cash)                  \
    X(Security)              \
    X(External)

MSTD_ENUM(AccountKind, std::uint8_t, ACCOUNT_KIND_LIST);

#define ACCOUNT_STATUS_LIST(X) \
    X(Active)                  \
    X(Closed)

MSTD_ENUM(AccountStatus, uint8_t, ACCOUNT_STATUS_LIST);

#define TRANSACTION_STATUS_LIST(X) \
    X(Completed)                   \
    X(Deleted)

MSTD_ENUM(TransactionStatus, std::uint8_t, TRANSACTION_STATUS_LIST);

#define TRANSACTION_TYPE_LIST(X) \
    X(Deposit)                   \
    X(Withdrawal)                \
    X(Stock)

MSTD_ENUM(TransactionType, std::uint8_t, TRANSACTION_TYPE_LIST);

#define TRANSACTION_DATA_TYPE_LIST(X) \
    X(Cash)                           \
    X(Trade)

MSTD_ENUM(TransactionDataType, std::uint8_t, TRANSACTION_DATA_TYPE_LIST);

#define ASSET_CLASS_LIST(X) \
    X(Stock)                \
    X(Etf)                  \
    X(Crypto)               \
    X(MutualFund)           \
    X(Future)               \
    X(Unknown)

MSTD_ENUM(AssetClass, std::uint8_t, ASSET_CLASS_LIST)

// NOLINTEND(cppcoreguidelines-macro-usage)

using micro_units = std::int64_t;

#endif   // __CONFIG__INCLUDE__CONFIG__FINANCE_HPP__