#ifndef __SQL_MODELS__INCLUDE__SQL_MODELS__TRANSACTION_OPTION_ROW_HPP__
#define __SQL_MODELS__INCLUDE__SQL_MODELS__TRANSACTION_OPTION_ROW_HPP__

#include "config/id_types.hpp"
#include "orm/orm_model.hpp"
#include "transaction_row.hpp"

/**
 * @brief Represents a row in the "transaction_option" database table, which
 * represents an option associated with a financial transaction. Each entry in
 * this table is linked to a specific transaction and contains details about the
 * option, such as whether it is a buy or sell position, the action taken on the
 * option (e.g., open, close, roll), and any rolled options if applicable. This
 * structured representation allows for detailed tracking and management of
 * options within financial transactions, enabling accurate reporting and
 * analysis of option-related activities.
 *
 */
struct TransactionOptionRow : public orm::ORMModel<"transaction_option">
{
    /// The id field, this is the primary key of the table and is
    /// auto-incremented
    ORM_FIELD(id, IdField<TransactionOptionId>)

    /// The transaction_id field, this is a required field and is a foreign key
    /// referencing the transaction table, the deletion behavior is set to
    /// cascade, meaning that if a transaction is deleted, all associated
    /// transaction options will also be deleted.
    ORM_FIELD(
        transactionId,
        TransactionRow::template ForeignId<tableName, orm::CascadeDelete>
    )

    /// Indicates whether the option is a buy or sell, this is a field that
    /// specifies whether the option position is a long (buy) or short (sell)
    /// position, which is essential for understanding the option's risk profile
    /// and potential payoff, as well as for accurate financial reporting and
    /// analysis of the option's performance.
    ORM_FIELD(buySell, Field<"buy_sell", OptionBuySell, orm::not_null_t>)

    /// The action taken on the option, this field specifies the type of action
    /// taken on the option (e.g., open, close, roll), which is important for
    /// understanding the context of the option transaction and for accurate
    /// reporting and analysis of the option's performance and the overall
    /// transaction strategy.
    ORM_FIELD(action, Field<"action", TransactionOptionAction, orm::not_null_t>)

    /// The rolled_option field, this is an optional field that represents a
    /// foreign key referencing the id field of the transaction_option table,
    /// this field is used to link a rolled option to its original option,
    /// allowing for tracking of option rolls within transactions, and is
    /// important for understanding the evolution of an option position over
    /// time, as well as for accurate reporting and analysis of the option's
    /// performance and the overall transaction strategy, especially when
    /// options are rolled to new strike prices or expiration dates as part of a
    /// trading strategy.
    ORM_FIELD(
        rolledOption,
        orm::Field<
            "id",
            TransactionOptionId,
            tableName,
            orm::foreign_key_t<
                orm::RestrictDelete,
                TransactionOptionRow,
                TransactionOptionRow::idField>>
    )

    /// @cond DOXYGEN_IGNORE
    ORM_FIELDS(
        TransactionOptionRow,
        id,
        transactionId,
        buySell,
        action,
        rolledOption
    )
    /// @endcond
};

#endif   // __SQL_MODELS__INCLUDE__SQL_MODELS__TRANSACTION_OPTION_ROW_HPP__