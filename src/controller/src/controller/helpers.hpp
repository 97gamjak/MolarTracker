#ifndef __CONTROLLER__SRC__CONTROLLER__HELPERS_HPP__
#define __CONTROLLER__SRC__CONTROLLER__HELPERS_HPP__

#include <expected>
#include <memory>
#include <string>

namespace store
{
    class IStockStore;         // Forward declaration
    class ITransactionStore;   // Forward declaration
    class IPositionStore;      // Forward declaration
    class IOptionStore;        // Forward declaration
}   // namespace store

namespace drafts
{
    class CreateStockTransactionDraft;    // Forward declaration
    class CreateOptionTransactionDraft;   // Forward declaration
    class PositionDraft;                  // Forward declaration
}   // namespace drafts

namespace gateway
{
    class PositionGateway;   // Forward declaration
}   // namespace gateway

namespace controller
{
    std::expected<void, std::string> convertTickerToInstrumentId(
        drafts::CreateStockTransactionDraft&       draft,
        const std::shared_ptr<store::IStockStore>& stockStore
    );

    std::expected<void, std::string> convertTickerToInstrumentId(
        drafts::CreateOptionTransactionDraft&      draft,
        const std::shared_ptr<store::IStockStore>& stockStore
    );

}   // namespace controller

#endif   // __CONTROLLER__SRC__CONTROLLER__HELPERS_HPP__