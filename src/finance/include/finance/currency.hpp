#ifndef __FINANCE__SRC__FINANCE__CURRENCY_HPP__
#define __FINANCE__SRC__FINANCE__CURRENCY_HPP__

#include <config/finance.hpp>
#include <mstd/error.hpp>

namespace finance
{

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
        /// The number of micro units (e.g., millionths of a currency unit) for
        /// the currency, this is used for internal calculations to avoid
        /// floating-point precision issues, and allows us to represent amounts
        /// in the smallest possible unit for the currency
        static constexpr std::uint8_t microUnit = 6;
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
        /// The number of micro units (e.g., millionths of a currency unit) for
        /// the currency, this is used for internal calculations to avoid
        /// floating-point precision issues, and allows us to represent amounts
        /// in the smallest possible unit for the currency
        static constexpr std::uint8_t microUnit = 6;
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
        /// The number of micro units (e.g., millionths of a currency unit) for
        /// the currency, this is used for internal calculations to avoid
        /// floating-point precision issues, and allows us to represent amounts
        /// in the smallest possible unit for the currency
        static constexpr std::uint8_t microUnit = 6;
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
        /// The number of micro units (e.g., millionths of a currency unit) for
        /// the currency, this is used for internal calculations to avoid
        /// floating-point precision issues, and allows us to represent amounts
        /// in the smallest possible unit for the currency
        static constexpr std::uint8_t microUnit = 6;
    };

    [[nodiscard]] std::uint8_t getMicroUnit(Currency currency);
    [[nodiscard]] std::string  getSymbol(Currency currency);

}   // namespace finance

#endif   // __FINANCE__SRC__FINANCE__CURRENCY_HPP__