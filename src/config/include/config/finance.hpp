#ifndef __CONFIG__INCLUDE__CONFIG__FINANCE_HPP__
#define __CONFIG__INCLUDE__CONFIG__FINANCE_HPP__

#include <cstdint>
#include <format>
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

// TODO(97gamjak): move this to a cpp file
#if defined(_MSC_VER) && !defined(__clang__)
#include <intrin.h>
#endif

[[nodiscard]] inline micro_units mulDiv(
    micro_units lhs,
    micro_units rhs,
    micro_units divisor
)
{
#if defined(_MSC_VER) && !defined(__clang__)
    int64_t    high;
    const auto low = static_cast<uint64_t>(_mul128(a, b, &high));
    int64_t    remainder;
    return _div128(high, low, divisor, &remainder);
#else
    // clang-format off
    _Pragma("GCC diagnostic push")
    _Pragma("GCC diagnostic ignored \"-Wpedantic\"") 
    const auto result =
        static_cast<micro_units>(static_cast<__int128>(lhs) * rhs / divisor);
    _Pragma("GCC diagnostic pop")
    return result;
    // clang-format on
#endif
}

template <std::uint8_t N>
struct intPow
{
    static constexpr std::uint64_t value = 10 * intPow<N - 1>::value;
};

template <>
struct intPow<0>
{
    static constexpr std::uint64_t value = 1;
};

struct Quantity
{
    static constexpr std::uint8_t precision = 8;
    static constexpr micro_units  factor    = intPow<precision>::value;

    micro_units value;

    explicit Quantity(micro_units value_) : value(value_) {}

    double getValue() const
    {
        return static_cast<double>(value) / static_cast<double>(factor);
    }

    [[nodiscard]] micro_units toMicroUnits() const { return value; }

    [[nodiscard]]
    friend Quantity operator-(const Quantity& quantity)
    {
        return Quantity(-quantity.value);
    }

    [[nodiscard]]
    std::string toString() const
    {
        return std::format(
            "{:.{}f}",
            static_cast<double>(value) / static_cast<double>(factor),
            precision
        );
    }

    bool operator>(const Quantity& other) const { return value > other.value; }

    template <typename T>
    friend bool operator>(const Quantity& lhs, const T& rhs)
    {
        return lhs.getValue() > rhs;
    }
};

#endif   // __CONFIG__INCLUDE__CONFIG__FINANCE_HPP__