#ifndef __DRAFTS__INCLUDE__DRAFTS__STOCK_MAPPER_HPP__
#define __DRAFTS__INCLUDE__DRAFTS__STOCK_MAPPER_HPP__

#include <vector>

namespace finance
{
    class Stock;   // forward declaration
}   // namespace finance

namespace drafts
{
    class StockInfoDraft;   // forward declaration

    /**
     * @brief Mapper class for converting between Stock and Stock drafts for UI.
     */
    class StockMapper
    {
       public:
        static StockInfoDraft toStockInfoDraft(const finance::Stock& stock);

        static std::vector<StockInfoDraft> toStockInfoDrafts(
            const std::vector<finance::Stock>& stocks
        );
    };

}   // namespace drafts

#endif   // __DRAFTS__INCLUDE__DRAFTS__STOCK_MAPPER_HPP__
