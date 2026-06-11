#ifndef __SQL_MODELS__INCLUDE__SQL_MODELS__TRANSACTION_OPTION_ROW_HPP__
#define __SQL_MODELS__INCLUDE__SQL_MODELS__TRANSACTION_OPTION_ROW_HPP__

#include "config/id_types.hpp"
#include "orm/orm_model.hpp"
#include "transaction_row.hpp"

struct TransactionOptionRow : public orm::ORMModel<"transaction_option">
{
    ORM_FIELD(id, IdField<TransactionOptionId>)

    ORM_FIELD(
        transactionId,
        TransactionRow::
            template ForeignId<tableName, orm::CascadeDelete, "transaction_id">
    )
};

#endif   // __SQL_MODELS__INCLUDE__SQL_MODELS__TRANSACTION_OPTION_ROW_HPP__