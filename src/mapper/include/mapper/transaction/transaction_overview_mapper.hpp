#ifndef __MAPPER__INCLUDE__MAPPER__TRANSACTION__TRANSACTION_OVERVIEW_MAPPER_HPP__
#define __MAPPER__INCLUDE__MAPPER__TRANSACTION__TRANSACTION_OVERVIEW_MAPPER_HPP__

#include <vector>

#include "common/container/id_map.hpp"
#include "config/id_types.hpp"
#include "drafts/transaction/transaction_overview_draft.hpp"

namespace finance
{
    class Transactions;   // Forward declaration
    class Options;        // Forward declaration
}   // namespace finance

namespace mapper
{

    /**
     * @brief Maps transaction overviews from the finance domain to the drafts
     * domain.
     *
     */
    class TransactionOverviewMapper
    {
       public:
        static std::vector<drafts::StockTransactionOverview> toStock(
            const finance::Transactions&            transactions,
            const IdMap<InstrumentId, std::string>& instrumentNames
        );

        static std::vector<drafts::CashTransactionOverview> toCash(
            const finance::Transactions& transactions
        );

        static std::vector<drafts::OptionTransactionOverview> toOption(
            const finance::Transactions& transactions,
            const finance::Options&      options
        );
    };

}   // namespace mapper

#endif   // __MAPPER__INCLUDE__MAPPER__TRANSACTION__TRANSACTION_OVERVIEW_MAPPER_HPP__
