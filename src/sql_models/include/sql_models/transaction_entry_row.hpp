#ifndef __SQL_MODELS__INCLUDE__SQL_MODELS__TRANSACTION_ENTRY_ROW_HPP__
#define __SQL_MODELS__INCLUDE__SQL_MODELS__TRANSACTION_ENTRY_ROW_HPP__

#include "account_row.hpp"
#include "config/finance.hpp"
#include "config/id_types.hpp"
#include "instrument_row.hpp"
#include "orm/constraints.hpp"
#include "orm/field.hpp"
#include "orm/orm_model.hpp"
#include "transaction_row.hpp"

/**
 * @brief Represents a row in the "transaction_entry" database table, which
 * represents an individual entry in a financial transaction. Each transaction
 * can have multiple entries, and each entry is associated with a specific
 * account and instrument. The transaction entry includes the amount of the
 * transaction for that account and instrument, as well as foreign key
 * references to the transaction, account, and instrument tables.
 *
 */
struct TransactionEntryRow : public orm::ORMModel<"tx_entry">
{
    /// The id field, this is the primary key of the table and is
    /// auto-incremented
    ORM_FIELD(id, IdField<TransactionEntryId>)

    /// The transaction_id field, this is a required field and is a foreign key
    /// referencing the transaction table, the deletion behavior is set to
    /// cascade, meaning that if a transaction is deleted, all associated
    /// transaction entries will also be deleted
    ORM_FIELD(
        transactionId,
        Field<
            "transaction_id",
            TransactionId,
            orm::foreign_key_t<
                orm::CascadeDelete,
                TransactionRow,
                decltype(TransactionRow::id)>,
            orm::not_null_t>
    )

    /// The account_id field, this is a required field and is a foreign key
    /// referencing the account table, the deletion behavior is set to restrict,
    /// meaning that if an account is deleted, the deletion will be restricted
    /// if there are any associated transaction entries, this ensures that
    /// transaction entries are not left orphaned without an associated account
    ORM_FIELD(
        accountId,
        Field<
            "account_id",
            AccountId,
            orm::foreign_key_t<
                orm::RestrictDelete,
                AccountRow,
                decltype(AccountRow::id)>,
            orm::not_null_t>
    )

    /// The instrument_id field, this is a required field and is a foreign key
    /// referencing the instrument table, the deletion behavior is set to
    /// restrict, meaning that if an instrument is deleted, the deletion will be
    /// restricted if there are any associated transaction entries, this ensures
    /// that transaction entries are not left orphaned without an associated
    /// instrument
    ORM_FIELD(
        instrumentId,
        Field<
            "instrument_id",
            InstrumentId,
            orm::foreign_key_t<
                orm::RestrictDelete,
                InstrumentRow,
                decltype(InstrumentRow::id)>,
            orm::not_null_t>
    )

    /// The amount field, this is a required field that represents the amount of
    /// the transaction for the associated account and instrument, it is stored
    /// as an integer representing the amount as micro-units (e.g., 0.001 cents
    /// for USD) to avoid floating-point precision issues
    ORM_FIELD(amount, Field<"amount", micro_units, orm::not_null_t>)

    /// @cond DOXYGEN_IGNORE
    ORM_FIELDS(
        TransactionEntryRow,
        id,
        transactionId,
        accountId,
        instrumentId,
        amount
    )
    /// @endcond

    [[nodiscard]]
    static orm::WhereExpr hasTransactionId(TransactionId transactionId);
};

#endif   // __SQL_MODELS__INCLUDE__SQL_MODELS__TRANSACTION_ENTRY_ROW_HPP__