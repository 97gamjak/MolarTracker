#ifndef __SQL_MODELS__INCLUDE__SQL_MODELS__PROFILE_ROW_HPP__
#define __SQL_MODELS__INCLUDE__SQL_MODELS__PROFILE_ROW_HPP__

#include <optional>
#include <string>

#include "config/id_types.hpp"
#include "orm/constraints.hpp"
#include "orm/field.hpp"
#include "orm/fixed_string.hpp"

/**
 * @brief Represents a row in the "profile" database table
 *
 */
struct ProfileRow final
{
    /// The name of the database table this struct represents
    static constexpr std::string tableName = "profile";

    /// The id field, this is the primary key of the table and is
    /// auto-incremented
    ORM_FIELD(id, orm::IdField<ProfileId>)

    /// The name field, this is a required and unique field
    ORM_FIELD(
        name,
        orm::Field<"name", std::string, orm::not_null_t, orm::unique_t>
    )

    /// The email field, this is an optional field
    ORM_FIELD(email, orm::Field<"email", std::optional<std::string>>)

    explicit ProfileRow() = default;
    explicit ProfileRow(ProfileId _id);

    ORM_FIELDS(ProfileRow, id, name, email)
};

#endif   // __SQL_MODELS__INCLUDE__SQL_MODELS__PROFILE_ROW_HPP__