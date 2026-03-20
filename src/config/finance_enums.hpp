#ifndef __CONFIG__FINANCE_ENUMS_HPP__
#define __CONFIG__FINANCE_ENUMS_HPP__

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

// NOLINTEND(cppcoreguidelines-macro-usage)

#endif   // __CONFIG__FINANCE_ENUMS_HPP__