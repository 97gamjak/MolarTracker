#ifndef __ORM__INCLUDE__ORM__JOIN_TPP__
#define __ORM__INCLUDE__ORM__JOIN_TPP__

#include "join.hpp"

namespace orm
{
    template <typename FromField, typename ToField>
    Join innerJoin()
    {
        Join join_;
        join_._type = JoinType::INNER;
        _fromFields<FromField, ToField>(join_);
        return join_;
    }

    template <typename FromField, typename ToField>
    Join join()
    {
        return innerJoin<FromField, ToField>();
    }

    template <typename FromField, typename ToField>
    Join leftJoin()
    {
        Join join_;
        join_._type = JoinType::LEFT;
        _fromFields<FromField, ToField>(join_);
        return join_;
    }

    template <typename FromField, typename ToField>
    Join rightJoin()
    {
        Join join_;
        join_._type = JoinType::RIGHT;
        _fromFields<FromField, ToField>(join_);
        return join_;
    }

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