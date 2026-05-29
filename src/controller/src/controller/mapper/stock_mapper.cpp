#include "stock_mapper.hpp"

#include "drafts/stock_draft.hpp"
#include "finance/instrument/stock.hpp"

namespace controller
{

    /**
     * @brief Convert a finance::Stock to a drafts::StockInfoDraft.
     *
     * @param stock
     * @return drafts::StockInfoDraft
     */
    drafts::StockInfoDraft StockMapper::toStockInfoDraft(
        const finance::Stock& stock
    )
    {
        return drafts::StockInfoDraft{
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
     * @brief Convert a vector of finance::Stock to a vector of
     * drafts::StockInfoDraft.
     *
     * @param stocks
     * @return std::vector<drafts::StockInfoDraft>
     */
    std::vector<drafts::StockInfoDraft> StockMapper::toStockInfoDrafts(
        const std::vector<finance::Stock>& stocks
    )
    {
        std::vector<drafts::StockInfoDraft> drafts;
        drafts.reserve(stocks.size());

        for (const auto& stock : stocks)
            drafts.push_back(toStockInfoDraft(stock));

        return drafts;
    }

}   // namespace controller
