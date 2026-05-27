#ifndef __CONTROLLER__SRC__CONTROLLER__HELPERS_HPP__
#define __CONTROLLER__SRC__CONTROLLER__HELPERS_HPP__

#include <expected>
#include <memory>
#include <string>
#include <vector>

#include "config/id_types.hpp"

namespace store
{
    class IStockStore;        // Forward declaration
    class TransactionStore;   // Forward declaration
    class PositionStore;      // Forward declaration
}   // namespace store

namespace drafts
{
    class CreateStockTransactionDraft;   // Forward declaration
    class PositionDraft;                 // Forward declaration
    class PositionDetailDraft;           // Forward declaration
}   // namespace drafts

namespace controller
{
    std::expected<void, std::string> convertTickerToInstrumentId(
        drafts::CreateStockTransactionDraft&       draft,
        const std::shared_ptr<store::IStockStore>& stockStore
    );

    std::vector<drafts::PositionDetailDraft> getOpenPositionDrafts(
        AccountId                                  account,
        const store::PositionStore&                positionStore,
        const std::shared_ptr<store::IStockStore>& stockStore,
        const store::TransactionStore&             transactionStore
    );

    std::vector<drafts::PositionDraft> getOpenPositionDrafts(
        const store::PositionStore&                positionStore,
        const std::shared_ptr<store::IStockStore>& stockStore,
        const store::TransactionStore&             transactionStore
    );

}   // namespace controller

#endif   // __CONTROLLER__SRC__CONTROLLER__HELPERS_HPP__