#ifndef __SQL_MODELS__PROFILE_ROW_HPP__
#define __SQL_MODELS__PROFILE_ROW_HPP__

#include <optional>
#include <string>
#include <tuple>

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
    static inline constexpr std::string table_name = "profile";

    orm::IdField<ProfileId>                                         id{};
    orm::Field<"name", std::string, orm::not_null_t, orm::unique_t> name{};
    orm::Field<"email", std::optional<std::string>>                 email{};

    // clang-format off
    [[nodiscard]] constexpr auto fields() { return std::tie(id, name, email); }
    [[nodiscard]] constexpr auto fields() const { return std::tie(id, name, email); }
    // clang-format on
};

#endif   // __SQL_MODELS__PROFILE_ROW_HPP__