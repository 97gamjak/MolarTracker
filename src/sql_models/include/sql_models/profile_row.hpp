#ifndef __SQL_MODELS__INCLUDE__SQL_MODELS__PROFILE_ROW_HPP__
#define __SQL_MODELS__INCLUDE__SQL_MODELS__PROFILE_ROW_HPP__

#include <optional>
#include <string>

#include "config/id_types.hpp"
#include "orm/constraints.hpp"
#include "orm/field.hpp"
#include "orm/fixed_string.hpp"
#include "orm/orm_model.hpp"
#include "orm/where_expr.hpp"

/**
 * @brief Represents a row in the "profile" database table
 *
 */
struct ProfileRow : public orm::ORMModel<"profile">
{
    /// The id field, this is the primary key of the table and is
    /// auto-incremented
    ORM_FIELD(id, IdField<ProfileId>)

    /// The name field, this is a required and unique field
    ORM_FIELD(name, Field<"name", std::string, orm::not_null_t, orm::unique_t>)

    /// The email field, this is an optional field
    ORM_FIELD(email, Field<"email", std::optional<std::string>>)

    ORM_FIELDS(ProfileRow, id, name, email)

    explicit ProfileRow() = default;
    explicit ProfileRow(ProfileId _id);

    [[nodiscard]] static orm::WhereExpr hasName(const std::string& name);
};

#endif   // __SQL_MODELS__INCLUDE__SQL_MODELS__PROFILE_ROW_HPP__