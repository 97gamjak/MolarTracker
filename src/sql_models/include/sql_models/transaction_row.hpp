#ifndef __SQL_MODELS__INCLUDE__SQL_MODELS__TRANSACTION_ROW_HPP__
#define __SQL_MODELS__INCLUDE__SQL_MODELS__TRANSACTION_ROW_HPP__

#include "config/finance.hpp"
#include "config/id_types.hpp"
#include "orm/constraints.hpp"
#include "orm/field.hpp"
#include "orm/fixed_string.hpp"
#include "orm/orm_model.hpp"
#include "utils/timestamp.hpp"

/**
 * @brief Represents a row in the "transaction" database table
 *
 */
struct TransactionRow : public orm::ORMModel<"transaction">
{
    /// The id field, this is the primary key of the table and is
    /// auto-incremented
    ORM_FIELD(id, IdField<TransactionId>)

    /// The timestamp field, this is a required field
    ORM_FIELD(timestamp, Field<"timestamp", Timestamp, orm::not_null_t>)

    /// The status field, this is a required field
    ORM_FIELD(status, Field<"status", TransactionStatus, orm::not_null_t>)

    /// The comment field, this is an optional field
    ORM_FIELD(comment, Field<"comment", std::optional<std::string>>)

    /// auto generate the fields() function using the ORM_FIELDS macro
    ORM_FIELDS(TransactionRow, id, timestamp, status, comment);
};

#endif   // __SQL_MODELS__INCLUDE__SQL_MODELS__TRANSACTION_ROW_HPP__