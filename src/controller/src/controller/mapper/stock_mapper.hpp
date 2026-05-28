#ifndef __CONTROLLER__SRC__CONTROLLER__MAPPER__STOCK_MAPPER_HPP__
#define __CONTROLLER__SRC__CONTROLLER__MAPPER__STOCK_MAPPER_HPP__

#include <vector>

namespace finance
{
    class Stock;   // forward declaration
}   // namespace finance

namespace drafts
{
    class StockInfoDraft;   // forward declaration
}   // namespace drafts

namespace controller
{
    /**
     * @brief Mapper class for converting between Stock and Stock drafts for UI.
     */
    class StockMapper
    {
       public:
        [[nodiscard]]
        static drafts::StockInfoDraft toStockInfoDraft(
            const finance::Stock& stock
        );

        [[nodiscard]]
        static std::vector<drafts::StockInfoDraft> toStockInfoDrafts(
            const std::vector<finance::Stock>& stocks
        );
    };

}   // namespace controller

#endif   // __CONTROLLER__SRC__CONTROLLER__MAPPER__STOCK_MAPPER_HPP__
