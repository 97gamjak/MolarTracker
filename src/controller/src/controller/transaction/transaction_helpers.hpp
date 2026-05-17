#ifndef __CONTROLLER__SRC__CONTROLLER__TRANSACTION__TRANSACTION_HELPERS_HPP__
#define __CONTROLLER__SRC__CONTROLLER__TRANSACTION__TRANSACTION_HELPERS_HPP__

#include <expected>
#include <string>

namespace app
{
    class StockStore;   // Forward declaration
}   // namespace app

namespace drafts
{
    class CreateStockTransactionDraft;   // Forward declaration
}   // namespace drafts

namespace controller
{
    std::expected<void, std::string> convertTickerToInstrumentId(
        drafts::CreateStockTransactionDraft& draft,
        const app::StockStore&               stockStore
    );
}   // namespace controller

#endif   // __CONTROLLER__SRC__CONTROLLER__TRANSACTION__TRANSACTION_HELPERS_HPP__