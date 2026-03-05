#ifndef __SQL_MODELS__PROFILE_ROW_IMPL_HPP__
#define __SQL_MODELS__PROFILE_ROW_IMPL_HPP__

#include "profile_row.hpp"

/**
 * @brief Returns a tuple of references to the fields of the ProfileRow
 *
 * @return constexpr auto
 */
constexpr auto ProfileRow::fields() { return std::tie(id, name, email); }

/**
 * @brief Returns a tuple of references to the fields of the ProfileRow (const
 * version)
 *
 * @return constexpr auto
 */
constexpr auto ProfileRow::fields() const { return std::tie(id, name, email); }

/**
 * @brief Get the Unique Groups object
 *
 * @return constexpr auto
 */
constexpr auto ProfileRow::getUniqueGroups()
{
    return orm::unique_set(
        orm::unique<(&ProfileRow::name), (&ProfileRow::email)>()
    );
}

#endif   // __SQL_MODELS__PROFILE_ROW_IMPL_HPP__