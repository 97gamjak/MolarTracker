#ifndef __DRAFTS__INCLUDE__DRAFTS__STOCK_DRAFT_HPP__
#define __DRAFTS__INCLUDE__DRAFTS__STOCK_DRAFT_HPP__

#include <string>

#include "config/finance.hpp"

namespace drafts
{

    /**
     * @brief Draft representation for stock information.
     *
     */
    class StockInfoDraft
    {
       private:
        /// Ticker symbol of the stock
        std::string _ticker;
        /// Short name of the stock
        std::string _shortName;
        /// Exchange where the stock is listed
        std::string _exchange;
        /// Currency of the stock
        Currency _currency;
        /// Industry of the stock
        std::string _industry;
        /// Sector of the stock
        std::string _sector;
        /// Asset class of the stock
        AssetClass _assetClass{AssetClass::Unknown};

       public:
        explicit StockInfoDraft(
            std::string ticker,
            std::string shortName,
            std::string exchange,
            Currency    currency,
            std::string industry,
            std::string sector,
            AssetClass  assetClass
        );

        [[nodiscard]] std::string getTicker() const;
        [[nodiscard]] std::string getShortName() const;
        [[nodiscard]] std::string getExchange() const;
        [[nodiscard]] Currency    getCurrency() const;
        [[nodiscard]] std::string getIndustry() const;
        [[nodiscard]] std::string getSector() const;
        [[nodiscard]] AssetClass  getAssetClass() const;
    };

}   // namespace drafts

#endif   // __DRAFTS__INCLUDE__DRAFTS__STOCK_DRAFT_HPP__