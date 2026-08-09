#include "sql_models/account_row.hpp"

/**
 * @brief Get a WhereExpr for filtering accounts by profile ID
 *
 * @param profileId
 * @return orm::WhereExpr
 */
orm::WhereExpr AccountRow::hasProfileId(const ProfileId& profileId)
{
    return orm::makeWhere<profileIdField>(profileId, filter::Operator::Equal);
}

/**
 * @brief Get a WhereExpr for filtering accounts by account ID
 *
 * @param accountId
 * @return orm::WhereExpr
 */
orm::WhereExpr AccountRow::hasId(const AccountId& accountId)
{
    return orm::makeWhere<idField>(accountId, filter::Operator::Equal);
}

/**
 * @brief Get a WhereExpr for filtering cash account details by account ID
 *
 * @param accountId
 * @return orm::WhereExpr
 */
orm::WhereExpr CashAccountDetailRow::hasId(const AccountId& accountId)
{
    return orm::makeWhere<idField>(accountId, filter::Operator::Equal);
}
