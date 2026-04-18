#include "sql_models/account_row.hpp"

/**
 * @brief Get a WhereExpr for filtering accounts by profile ID
 *
 * @param profileId
 * @return orm::WhereExpr
 */
orm::WhereExpr AccountRow::hasProfileId(const ProfileId& profileId)
{
    return orm::makeWhere(profileIdField{profileId}, filter::Operator::Equal);
}