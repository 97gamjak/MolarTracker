#ifndef __ORM__INCLUDE__ORM__JOIN_TPP__
#define __ORM__INCLUDE__ORM__JOIN_TPP__

#include "join.hpp"

namespace orm
{
    template <typename FromField, typename ToField>
    Join innerJoin()
    {
        Join join;
        join._type = JoinType::INNER;
        _fromFields<FromField, ToField>(join);
        return join;
    }

    template <typename FromField, typename ToField>
    Join join()
    {
        return innerJoin<FromField, ToField>();
    }

    template <typename FromField, typename ToField>
    Join leftJoin()
    {
        Join join;
        join._type = JoinType::LEFT;
        _fromFields<FromField, ToField>(join);
        return join;
    }

    template <typename FromField, typename ToField>
    Join rightJoin()
    {
        Join join;
        join._type = JoinType::RIGHT;
        _fromFields<FromField, ToField>(join);
        return join;
    }

    template <typename FromField, typename ToField>
    void _fromFields(Join& join)
    {
        join._fromTable = FromField::tableName;
        join._fromField = FromField::name;
        join._toTable   = ToField::tableName;
        join._toField   = ToField::name;
    }
}   // namespace orm

#endif   // __ORM__INCLUDE__ORM__JOIN_TPP__