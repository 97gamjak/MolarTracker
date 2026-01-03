// sql_models/profile_row.hpp
#pragma once

#include <optional>
#include <string>
#include <tuple>

#include "config/id_types.hpp"
#include "orm/field.hpp"
#include "orm/fixed_string.hpp"
#include "orm/tags.hpp"

struct ProfileRow final
{
    static constexpr orm::fixed_string table_name = "profile";

    orm::Field<"id", ProfileId, orm::primary_key_t, orm::auto_increment_t> id{};
    orm::Field<"name", std::string, orm::not_null_t> name{};
    orm::Field<"email", std::optional<std::string>>  email{};

    [[nodiscard]] constexpr auto fields() noexcept
    {
        return std::tie(id, name, email);
    }
    [[nodiscard]] constexpr auto fields() const noexcept
    {
        return std::tie(id, name, email);
    }
};
