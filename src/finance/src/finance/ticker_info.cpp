#include "finance/ticker_info.hpp"

namespace finance
{
    AssetClass fromQuote(std::string_view quote)
    {
        if (quote == "Stock")
            return AssetClass::Stock;
        if (quote == "Etf")
            return AssetClass::Etf;
        if (quote == "Crypto")
            return AssetClass::Crypto;
        if (quote == "MutualFund")
            return AssetClass::MutualFund;
        if (quote == "Future")
            return AssetClass::Future;

        return AssetClass::Unknown;
    }

    std::string toString(AssetClass assetClass)
    {
        if (assetClass == AssetClass::MutualFund)
            return "MutualFund";

        return AssetClassMeta::toString(assetClass);
    }
}   // namespace finance