#include "finance/currency.hpp"

#include <stdexcept>

namespace finance
{
    std::uint8_t getMicroUnit(Currency currency)
    {
        switch (currency)
        {
            case Currency::USD:
                return CurrencyTraits<Currency::USD>::microUnit;
            case Currency::EUR:
                return CurrencyTraits<Currency::EUR>::microUnit;
            case Currency::GBP:
                return CurrencyTraits<Currency::GBP>::microUnit;
            case Currency::CHF:
                return CurrencyTraits<Currency::CHF>::microUnit;
        }

        throw std::invalid_argument("Unsupported currency");
    }

    std::string getSymbol(Currency currency)
    {
        switch (currency)
        {
            case Currency::USD:
                return CurrencyTraits<Currency::USD>::symbol;
            case Currency::EUR:
                return CurrencyTraits<Currency::EUR>::symbol;
            case Currency::GBP:
                return CurrencyTraits<Currency::GBP>::symbol;
            case Currency::CHF:
                return CurrencyTraits<Currency::CHF>::symbol;
        }

        throw std::invalid_argument("Unsupported currency");
    }
}   // namespace finance
