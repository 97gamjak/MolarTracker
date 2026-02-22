#ifndef __SQL_MODELS__PROFILE_ROW_HPP__
#define __SQL_MODELS__PROFILE_ROW_HPP__

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
   public:   // fields
    /// The name of the database table this struct represents
    static constexpr std::string table_name = "profile";

    /// The id field, this is the primary key of the table and is
    /// auto-incremented
    orm::IdField<ProfileId> id;

    /// The name field, this is a required and unique field
    orm::Field<"name", std::string, orm::not_null_t, orm::unique_t> name;

    /// The email field, this is an optional field
    orm::Field<"email", std::optional<std::string>> email;

   public:   // methods
    [[nodiscard]] constexpr auto fields();
    [[nodiscard]] constexpr auto fields() const;
};

#ifndef __SQL_MODELS__PROFILE_ROW_IMPL_HPP__
#include "profile_row.impl.hpp"   // IWYU pragma: keep
#endif

#endif   // __SQL_MODELS__PROFILE_ROW_HPP__