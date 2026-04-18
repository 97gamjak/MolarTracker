#ifndef __DRAFTS__INCLUDE__DRAFTS__TRANSACTION_MAPPER_HPP__
#define __DRAFTS__INCLUDE__DRAFTS__TRANSACTION_MAPPER_HPP__

#include "drafts/transaction_draft.hpp"
#include "finance/transaction.hpp"

namespace drafts
{

    class TransactionMapper
    {
       public:
        static TransactionDraft toDraft(
            const finance::Transaction& transaction
        );

        static finance::Transaction toTransaction(
            const TransactionDraft& draft,
            TransactionId           id
        );
    };

}   // namespace drafts

#endif   // __DRAFTS__INCLUDE__DRAFTS__TRANSACTION_MAPPER_HPP__