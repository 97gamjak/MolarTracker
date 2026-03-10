#ifndef __SQL_MODELS__ACCOUNT_ROW_HPP__
#define __SQL_MODELS__ACCOUNT_ROW_HPP__

#include <string>

#include "config/finance_enums.hpp"
#include "config/id_types.hpp"
#include "orm/field.hpp"
#include "orm/fixed_string.hpp"

/**
 * @brief Represents a row in the "account" database table, which serves as a
 * base for different types of financial accounts. This table includes a primary
 * key and a field for the kind of account, which can be used to distinguish
 * between different types of accounts (e.g., cash, security, etc.). The
 * specific details of each account type can be stored in separate tables that
 * extend this base table.
 */
struct AccountRow
{
    /// The name of the database table this struct represents
    static constexpr std::string tableName = "account";

    /// as we have a 1:1 relationship between AccountRow and CashAccountRow, we
    /// disallow inserting an AccountRow without a corresponding CashAccountRow
    using insert_policy = orm::requires_paired_insert_t;

    /// The id field, this is the primary key of the table and is
    /// auto-incremented
    orm::IdField<AccountId> id;

    /// The kind field, this indicates the type of account (e.g., cash,
    /// security, etc.) and is a required field
    orm::Field<"kind", AccountKind, orm::not_null_t> kind;

    [[nodiscard]] constexpr auto fields();
    [[nodiscard]] constexpr auto fields() const;
};

/**
 * @brief Represents a row in the "cash_account" database table
 *
 */
struct CashAccountRow final
{
   public:   // fields
    /// The name of the database table this struct represents
    static constexpr std::string tableName = "cash_account";

    /// as we have a 1:1 relationship between AccountRow and CashAccountRow, we
    /// disallow inserting a CashAccountRow without a corresponding AccountRow
    using insert_policy = orm::requires_paired_insert_t;

    /// The id field, this is the primary key of the table and is also a foreign
    /// key referencing the id field of the AccountRow table, this indicates
    /// that each cash account is associated with a specific account entry, and
    /// the deletion behavior is set to cascade, meaning that if an account
    /// entry is deleted, the associated cash account entry will also be deleted
    orm::Field<
        "id",
        AccountId,
        orm::primary_key_t,
        orm::foreign_key_t<
            orm::CascadeDelete,
            AccountRow,
            decltype(AccountRow::id)>>
        id;

    /// The status field, this is a required field
    orm::Field<"status", AccountStatus, orm::not_null_t> status;
    /// The profile_id field, this is a required field and is a foreign key
    /// referencing the profile table
    orm::Field<"profile_id", ProfileId, orm::not_null_t> profileId;
    /// The currency field, this is a required field
    orm::Field<"currency", Currency, orm::not_null_t> currency;
    /// The name field, this is a required field
    orm::Field<"name", std::string, orm::not_null_t> name;

   public:   // methods
    [[nodiscard]] constexpr auto fields();
    [[nodiscard]] constexpr auto fields() const;
};

#ifndef __SQL_MODELS__ACCOUNT_ROW_IMPL_HPP__
#include "account_row.impl.hpp"   // IWYU pragma: keep
#endif

#endif   // __SQL_MODELS__ACCOUNT_ROW_HPP__