#ifndef __SQL_MODELS__CASH_ACCOUNT_ROW_HPP__
#define __SQL_MODELS__CASH_ACCOUNT_ROW_HPP__

#include <string>

#include "config/id_types.hpp"
#include "finance/cash_account.hpp"
#include "finance/currency.hpp"
#include "orm/field.hpp"
#include "orm/fixed_string.hpp"

/**
 * @brief Represents a row in the "cash_account" database table
 *
 */
struct CashAccountRow final
{
   public:   // fields
    /// The name of the database table this struct represents
    static constexpr std::string table_name = "cash_account";

    /// The id field, this is the primary key of the table and is
    /// auto-incremented
    orm::IdField<AccountId> id;
    /// The type field, this is a required field
    orm::Field<"type", finance::AccountType, orm::not_null_t> type;
    /// The status field, this is a required field
    orm::Field<"status", finance::AccountStatus, orm::not_null_t> status;
    /// The profile_id field, this is a required field and is a foreign key
    /// referencing the profile table
    orm::Field<"profile_id", ProfileId, orm::not_null_t> profileId;
    /// The currency field, this is a required field
    orm::Field<"currency", finance::Currency, orm::not_null_t> currency;
    /// The name field, this is a required field
    orm::Field<"name", std::string, orm::not_null_t> name;

   public:   // methods
    [[nodiscard]] constexpr auto fields();
    [[nodiscard]] constexpr auto fields() const;
};

#ifndef __SQL_MODELS__CASH_ACCOUNT_ROW_IMPL_HPP__
#include "cash_account_row.impl.hpp"   // IWYU pragma: keep
#endif

#endif   // __SQL_MODELS__CASH_ACCOUNT_ROW_HPP__