#ifndef __FINANCE_CURRENCY_HPP__
#define __FINANCE_CURRENCY_HPP__

#include <cstdint>

#include "mstd/enum.hpp"
#include "mstd/error.hpp"

namespace finance
{

#define CURRENCY_LIST(X) \
    X(USD)               \
    X(EUR)               \
    X(GBP)               \
    X(CHF)

    MSTD_ENUM(Currency, std::uint8_t, CURRENCY_LIST);

    /**
     * @brief Traits for each currency, providing information such as symbol,
     * full name, and minor unit. The default template will cause a compile-time
     * error if a specialization is not provided for a currency.
     *
     * @tparam C the currency type
     */
    template <Currency C>
    struct CurrencyTraits
    {
        /// Trigger a compile-time error if this template is instantiated
        /// without a specialization for the given currency
        MSTD_COMPILE_FAIL("CurrencyTraits not defined for this currency");
    };

    /**
     * @brief CurrencyTraits specialization for USD
     *
     * @tparam Currency::USD
     */
    template <>
    struct CurrencyTraits<Currency::USD>
    {
        /// The symbol for the currency
        static constexpr const char* symbol = "$";
        /// The full name of the currency
        static constexpr const char* fullName = "US Dollar";
        /// The number of minor units (e.g., cents) for the currency
        static constexpr std::uint8_t minorUnit = 2;
    };

    /**
     * @brief CurrencyTraits specialization for EUR
     *
     * @tparam Currency::EUR
     */
    template <>
    struct CurrencyTraits<Currency::EUR>
    {
        /// The symbol for the currency
        static constexpr const char* symbol = "€";
        /// The full name of the currency
        static constexpr const char* fullName = "Euro";
        /// The number of minor units (e.g., cents) for the currency
        static constexpr std::uint8_t minorUnit = 2;
    };

    /**
     * @brief CurrencyTraits specialization for GBP
     *
     * @tparam Currency::GBP
     */
    template <>
    struct CurrencyTraits<Currency::GBP>
    {
        /// The symbol for the currency
        static constexpr const char* symbol = "£";
        /// The full name of the currency
        static constexpr const char* fullName = "British Pound";
        /// The number of minor units (e.g., cents) for the currency
        static constexpr std::uint8_t minorUnit = 2;
    };

    /**
     * @brief CurrencyTraits specialization for CHF
     *
     * @tparam Currency::CHF
     */
    template <>
    struct CurrencyTraits<Currency::CHF>
    {
        /// The symbol for the currency
        static constexpr const char* symbol = "CHF";
        /// The full name of the currency
        static constexpr const char* fullName = "Swiss Franc";
        /// The number of minor units (e.g., cents) for the currency
        static constexpr std::uint8_t minorUnit = 2;
    };

}   // namespace finance

#endif   // __FINANCE_CURRENCY_HPP__