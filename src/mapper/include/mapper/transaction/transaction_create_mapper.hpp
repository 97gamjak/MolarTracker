#ifndef __CONTROLLER__SRC__CONTROLLER__MAPPER__TRANSACTION__TRANSACTION_CREATE_MAPPER_HPP__
#define __CONTROLLER__SRC__CONTROLLER__MAPPER__TRANSACTION__TRANSACTION_CREATE_MAPPER_HPP__

#include "finance/transaction/cash_transaction.hpp"
#include "finance/transaction/option_transaction.hpp"
#include "finance/transaction/stock_transaction.hpp"

namespace drafts
{
    class CreateCashTransactionDraft;
    class CreateStockTransactionDraft;
    class CreateOptionTransactionDraft;
}   // namespace drafts

namespace mapper
{

    /**
     * @brief Maps create transaction drafts to finance transactions.
     *
     */
    class TransactionCreateMapper
    {
       public:
        [[nodiscard]]
        static finance::CashTransaction fromCreateCashDraft(
            const drafts::CreateCashTransactionDraft& draft
        );

        [[nodiscard]]
        static finance::StockTransaction fromCreateStockDraft(
            const drafts::CreateStockTransactionDraft& draft
        );

        [[nodiscard]]
        static finance::OptionTransaction fromCreateOptionDraft(
            const drafts::CreateOptionTransactionDraft& draft
        );
    };

}   // namespace mapper

#endif   // __CONTROLLER__SRC__CONTROLLER__MAPPER__TRANSACTION__TRANSACTION_CREATE_MAPPER_HPP__
