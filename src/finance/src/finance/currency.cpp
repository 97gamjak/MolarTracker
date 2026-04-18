#include "finance/currency.hpp"

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
    }
}   // namespace finance
