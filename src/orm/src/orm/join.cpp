#include "orm/join.hpp"

#include "orm/orm_exception.hpp"

namespace orm
{
    std::string Join::toSQL() const
    {
        std::string sqlText;

        switch (_type)
        {
            case JoinType::INNER:
                sqlText += "INNER JOIN ";
                break;
            case JoinType::LEFT:
                sqlText += "LEFT JOIN ";
                break;
            case JoinType::RIGHT:
                sqlText += "RIGHT JOIN ";
                break;
        }

        sqlText += _toTable;
        sqlText += " ON ";
        sqlText += _fromTable + "." + _fromField;
        sqlText += " = ";
        sqlText += _toTable + "." + _toField;

        return sqlText;
    }

    Joins& Joins::add(const Join& join)
    {
        _joins.push_back(join);
        return *this;
    }

    std::string Joins::toSQL() const
    {
        std::string sql;
        for (const auto& join : _joins)
        {
            sql += " ";
            sql += join.toSQL();
        }
        return sql;
    }

}   // namespace orm