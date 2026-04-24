#ifndef __SQL_MODELS__INCLUDE__SQL_MODELS__ACCOUNT_ROW_HPP__
#define __SQL_MODELS__INCLUDE__SQL_MODELS__ACCOUNT_ROW_HPP__

#include <string>

#include "config/finance.hpp"
#include "config/id_types.hpp"
#include "orm/field.hpp"
#include "orm/fixed_string.hpp"
#include "orm/orm_model.hpp"
#include "orm/type_traits.hpp"
#include "orm/where_expr.hpp"
#include "profile_row.hpp"

/**
 * @brief Represents a row in the "account" database table, which serves as a
 * base for different types of financial accounts. This table includes a primary
 * key and a field for the kind of account, which can be used to distinguish
 * between different types of accounts (e.g., cash, security, etc.). The
 * specific details of each account type can be stored in separate tables that
 * extend this base table.
 */
struct AccountRow : orm::ORMModel<"account">
{
    /// as we have a 1:1 relationship between AccountRow and CashAccountRow, we
    /// disallow inserting an AccountRow without a corresponding CashAccountRow
    using insert_policy = orm::requires_paired_insert_t;

    /// The id field, this is the primary key of the table and is
    /// auto-incremented
    ORM_FIELD(id, IdField<AccountId>)

    /// The kind field, this indicates the type of account (e.g., cash,
    /// security, etc.) and is a required field
    ORM_FIELD(kind, Field<"kind", AccountKind, orm::not_null_t>)

    /// The profile_id field, this is a required field and is a foreign key
    /// referencing the profile table
    ORM_FIELD(
        profileId,
        Field<
            "profile_id",
            ProfileId,
            orm::foreign_key_t<
                orm::RestrictDelete,
                ProfileRow,
                ProfileRow::idField>>
    )

    /// The name field, this is a required field
    ORM_FIELD(name, Field<"name", std::string, orm::not_null_t>)

    /// The status field, this is a required field
    ORM_FIELD(status, Field<"status", AccountStatus, orm::not_null_t>)

    /// The currency field, this is a required field
    ORM_FIELD(currency, Field<"currency", Currency, orm::not_null_t>)

    ORM_FIELDS(AccountRow, id, kind, profileId, name, status, currency)

    /**
     * @brief Get the Unique Groups object
     *
     * @return auto
     */
    static auto getUniqueGroups()
    {
        return orm::unique_set(
            orm::unique_group<
                &AccountRow::profileId,
                &AccountRow::kind,
                &AccountRow::name>()
        );
    }

    [[nodiscard]] static orm::WhereExpr hasProfileId(
        const ProfileId& profileId
    );
};

#endif   // __SQL_MODELS__INCLUDE__SQL_MODELS__ACCOUNT_ROW_HPP__