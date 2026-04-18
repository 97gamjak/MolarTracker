#include "sql_models/profile_row.hpp"

#include "orm/where_expr.hpp"

/**
 * @brief Construct a new Profile Row:: Profile Row object
 *
 * @param _id
 */
ProfileRow::ProfileRow(ProfileId _id) : id{_id} {}

/**
 * @brief Get a WhereExpr for filtering profiles by name
 *
 * @param name
 * @return orm::WhereExpr
 */
orm::WhereExpr ProfileRow::hasName(const std::string& name)
{
    return orm::makeWhere(nameField{name}, filter::Operator::Equal);
}
