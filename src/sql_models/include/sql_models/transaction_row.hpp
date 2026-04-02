#ifndef __SQL_MODELS__INCLUDE__SQL_MODELS__TRANSACTION_ROW_HPP__
#define __SQL_MODELS__INCLUDE__SQL_MODELS__TRANSACTION_ROW_HPP__

#include "config/finance_enums.hpp"
#include "config/id_types.hpp"
#include "orm/constraints.hpp"
#include "orm/field.hpp"
#include "orm/fixed_string.hpp"
#include "utils/timestamp.hpp"

/**
 * @brief Represents a row in the "transaction" database table
 *
 */
struct TransactionRow final
{
   public:   // fields
    /// The name of the database table this struct represents
    /// The "_" is needed because transaction is a reserved keyword in SQLite
    static constexpr std::string tableName = "transaction_";

    /// The id field, this is the primary key of the table and is
    /// auto-incremented
    orm::IdField<TransactionId> id;

    /// The timestamp field, this is a required field
    orm::Field<"timestamp", Timestamp, orm::not_null_t> timestamp;

    /// The status field, this is a required field
    orm::Field<"status", TransactionStatus, orm::not_null_t> status;

    /// The comment field, this is an optional field
    orm::Field<"comment", std::optional<std::string>> comment;

    /// auto generate the fields() function using the ORM_FIELDS macro
    ORM_FIELDS(TransactionRow, id, timestamp, status, comment);
};

#endif   // __SQL_MODELS__INCLUDE__SQL_MODELS__TRANSACTION_ROW_HPP__