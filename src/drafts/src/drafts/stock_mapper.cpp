#include "drafts/stock_mapper.hpp"

#include "drafts/stock_draft.hpp"
#include "finance/stock.hpp"

namespace drafts
{

    /**
     * @brief Convert a finance::Stock to a StockInfoDraft.
     *
     * @param stock
     * @return StockInfoDraft
     */
    StockInfoDraft StockMapper::toStockInfoDraft(const finance::Stock& stock)
    {
        return StockInfoDraft{
            stock.getTicker(),
            stock.getShortName(),
            stock.getLongName(),
            stock.getExchange(),
            stock.getCurrency(),
            stock.getIndustry(),
            stock.getSector(),
            stock.getAssetClass()
        };
    }

    /**
     * @brief Convert a vector of finance::Stock to a vector of StockInfoDraft.
     *
     * @param stocks
     * @return std::vector<StockInfoDraft>
     */
    std::vector<StockInfoDraft> StockMapper::toStockInfoDrafts(
        const std::vector<finance::Stock>& stocks
    )
    {
        std::vector<StockInfoDraft> drafts;
        drafts.reserve(stocks.size());

        for (const auto& stock : stocks)
            drafts.push_back(toStockInfoDraft(stock));

        return drafts;
    }

}   // namespace drafts
