#ifndef __ORM__INCLUDE__ORM__JOIN_TPP__
#define __ORM__INCLUDE__ORM__JOIN_TPP__

#include "join.hpp"

namespace orm
{
    /**
     * @brief Create an inner join between two tables
     *
     * @tparam FromField The field from the "from" table
     * @tparam ToField The field from the "to" table
     * @return Join The constructed inner join
     */
    template <typename FromField, typename ToField>
    Join innerJoin()
    {
        Join join_{JoinType::INNER};
        _fromFields<FromField, ToField>(join_);
        return join_;
    }

    /**
     * @brief Create a join between two tables
     *
     * @tparam FromField The field from the "from" table
     * @tparam ToField The field from the "to" table
     * @return Join The constructed join
     */
    template <typename FromField, typename ToField>
    Join join()
    {
        return innerJoin<FromField, ToField>();
    }

    /**
     * @brief Create a left join between two tables
     *
     * @tparam FromField The field from the "from" table
     * @tparam ToField The field from the "to" table
     * @return Join The constructed left join
     */
    template <typename FromField, typename ToField>
    Join leftJoin()
    {
        Join join_{JoinType::LEFT};
        _fromFields<FromField, ToField>(join_);
        return join_;
    }

    /**
     * @brief Create a right join between two tables
     *
     * @tparam FromField The field from the "from" table
     * @tparam ToField The field from the "to" table
     * @return Join The constructed right join
     */
    template <typename FromField, typename ToField>
    Join rightJoin()
    {
        Join join_{JoinType::RIGHT};
        _fromFields<FromField, ToField>(join_);
        return join_;
    }

    /**
     * @brief Set the fields for the "from" table in the join
     *
     * @tparam FromField The field from the "from" table
     * @tparam ToField The field from the "to" table
     * @param join_ The join object to modify
     */
    template <typename FromField, typename ToField>
    void _fromFields(Join& join_)
    {
        join_._fromTable = std::string(FromField::tableName);
        join_._fromField = std::string(FromField::name);
        join_._toTable   = std::string(ToField::tableName);
        join_._toField   = std::string(ToField::name);
    }
}   // namespace orm

#endif   // __ORM__INCLUDE__ORM__JOIN_TPP__