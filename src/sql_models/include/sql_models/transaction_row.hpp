#ifndef __SQL_MODELS__INCLUDE__SQL_MODELS__TRANSACTION_ROW_HPP__
#define __SQL_MODELS__INCLUDE__SQL_MODELS__TRANSACTION_ROW_HPP__

#include "common/finance.hpp"
#include "common/timestamp.hpp"
#include "config/id_types.hpp"
#include "orm/constraints.hpp"
#include "orm/field.hpp"
#include "orm/fixed_string.hpp"
#include "orm/orm_model.hpp"
#include "orm/where_expr.hpp"

/**
 * @brief Represents a row in the "transaction" database table
 *
 */
struct TransactionRow : public orm::ORMModel<"transaction_">
{
    static_assert(
        tableName != "transaction",
        "Table name must not be 'transaction', this is a reserved key in SQL"
    );

    [[nodiscard]]
    static orm::WhereExpr hasTransactionId(TransactionId transactionId);

    /// The id field, this is the primary key of the table and is
    /// auto-incremented
    ORM_FIELD(id, IdField<TransactionId>)

    /// The timestamp field, this is a required field
    ORM_FIELD(timestamp, Field<"timestamp", Timestamp, orm::not_null_t>)

    /// The status field, this is a required field
    ORM_FIELD(status, Field<"status", TransactionStatus, orm::not_null_t>)

    /// The comment field, this is an optional field
    ORM_FIELD(comment, Field<"comment", std::optional<std::string>>)

    ORM_FIELD(type, Field<"type", TransactionDataType, orm::not_null_t>)

    /// auto generate the fields() function using the ORM_FIELDS macro
    ORM_FIELDS(TransactionRow, id, timestamp, status, comment, type);

    /// Helper type alias for defining foreign key fields referencing the id
    /// field of the transaction table, this allows for concise definitions of
    /// foreign key fields that reference the transaction table with a specified
    /// deletion behavior (e.g., cascade or restrict)
    template <orm::fixed_string tableName, orm::DeletionType T>
    using ForeignId = orm::Field<
        "transaction_id",
        TransactionId,
        tableName,
        orm::foreign_key_t<T, TransactionRow, decltype(TransactionRow::id)>,
        orm::not_null_t>;
};

#endif   // __SQL_MODELS__INCLUDE__SQL_MODELS__TRANSACTION_ROW_HPP__