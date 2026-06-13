#ifndef __CONFIG__INCLUDE__CONFIG__FINANCE_HPP__
#define __CONFIG__INCLUDE__CONFIG__FINANCE_HPP__

#include <cstdint>
#include <mstd/enum.hpp>

// NOLINTBEGIN(cppcoreguidelines-macro-usage)

#define CURRENCY_LIST(X) \
    X(Unknown)           \
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
    X(Stock)                     \
    X(Option)

MSTD_ENUM(TransactionType, std::uint8_t, TRANSACTION_TYPE_LIST);

#define TRANSACTION_ENTRY_TYPE_LIST(X) \
    X(General)                         \
    X(Fees)

MSTD_ENUM(TransactionEntryType, std::uint8_t, TRANSACTION_ENTRY_TYPE_LIST);

#define TRANSACTION_DATA_TYPE_LIST(X) \
    X(Cash)                           \
    X(Stock)                          \
    X(Option)

MSTD_ENUM(TransactionDataType, std::uint8_t, TRANSACTION_DATA_TYPE_LIST);

constexpr int TxDataTypeNotImplError = 0;

#define ASSET_CLASS_LIST(X) \
    X(Stock)                \
    X(Etf)                  \
    X(Crypto)               \
    X(MutualFund)           \
    X(Future)               \
    X(Unknown)

MSTD_ENUM(AssetClass, std::uint8_t, ASSET_CLASS_LIST);

#define OPTION_TYPE_LIST(X) \
    X(Call)                 \
    X(Put)

MSTD_ENUM(OptionType, std::uint8_t, OPTION_TYPE_LIST);

#define OPTION_BUY_SELL_LIST(X) \
    X(Buy)                      \
    X(Sell)

MSTD_ENUM(OptionBuySell, std::uint8_t, OPTION_BUY_SELL_LIST);

#define TRANSACTION_OPTION_ACTION_LIST(X) \
    X(Open)                               \
    X(Close)                              \
    X(RollOpen)                           \
    X(RollClose)                          \
    X(Exercised)

MSTD_ENUM(
    TransactionOptionAction,
    std::uint8_t,
    TRANSACTION_OPTION_ACTION_LIST
);

// NOLINTEND(cppcoreguidelines-macro-usage)

/**
 * @brief Convert an AssetClass to its string representation.
 *
 * @param assetClass The AssetClass to convert.
 * @return std::string The string representation of the AssetClass.
 */
static inline std::string toString(AssetClass assetClass)
{
    if (assetClass == AssetClass::MutualFund)
        return "MutualFund";

    return AssetClassMeta::toString(assetClass);
}

#endif   // __CONFIG__INCLUDE__CONFIG__FINANCE_HPP__