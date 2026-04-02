#ifndef __SQL_MODELS__INCLUDE__SQL_MODELS__TRANSACTION_ENTRY_ROW_HPP__
#define __SQL_MODELS__INCLUDE__SQL_MODELS__TRANSACTION_ENTRY_ROW_HPP__

#include "account_row.hpp"
#include "config/id_types.hpp"
#include "finance/cash.hpp"
#include "instrument_row.hpp"
#include "orm/constraints.hpp"
#include "orm/field.hpp"
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
struct TransactionEntryRow final
{
    /// The name of the database table this struct represents
    static constexpr std::string tableName = "tx_entry";

    /// The id field, this is the primary key of the table and is
    /// auto-incremented
    orm::IdField<TransactionEntryId> id;

    /// The transaction_id field, this is a required field and is a foreign key
    /// referencing the transaction table, the deletion behavior is set to
    /// cascade, meaning that if a transaction is deleted, all associated
    /// transaction entries will also be deleted
    orm::Field<
        "transaction_id",
        TransactionId,
        orm::foreign_key_t<
            orm::CascadeDelete,
            TransactionRow,
            decltype(TransactionRow::id)>,
        orm::not_null_t>
        transactionId;

    /// The account_id field, this is a required field and is a foreign key
    /// referencing the account table, the deletion behavior is set to restrict,
    /// meaning that if an account is deleted, the deletion will be restricted
    /// if there are any associated transaction entries, this ensures that
    /// transaction entries are not left orphaned without an associated account
    orm::Field<
        "account_id",
        AccountId,
        orm::foreign_key_t<
            orm::RestrictDelete,
            AccountRow,
            decltype(AccountRow::id)>,
        orm::not_null_t>
        accountId;

    /// The instrument_id field, this is a required field and is a foreign key
    /// referencing the instrument table, the deletion behavior is set to
    /// restrict, meaning that if an instrument is deleted, the deletion will be
    /// restricted if there are any associated transaction entries, this ensures
    /// that transaction entries are not left orphaned without an associated
    /// instrument
    orm::Field<
        "instrument_id",
        InstrumentId,
        orm::foreign_key_t<
            orm::RestrictDelete,
            InstrumentRow,
            decltype(InstrumentRow::id)>,
        orm::not_null_t>
        instrumentId;

    /// The amount field, this is a required field that represents the amount of
    /// the transaction for the associated account and instrument, it is stored
    /// as an integer representing the amount as micro-units (e.g., 0.001 cents
    /// for USD) to avoid floating-point precision issues
    orm::Field<"amount", finance::micro_units, orm::not_null_t> amount;

    /// auto generate the fields() function using the ORM_FIELDS macro
    ORM_FIELDS(
        TransactionEntryRow,
        id,
        transactionId,
        accountId,
        instrumentId,
        amount
    );
};

#endif   // __SQL_MODELS__INCLUDE__SQL_MODELS__TRANSACTION_ENTRY_ROW_HPP__