#include "orm/join.hpp"

namespace orm
{
    /**
     * @brief Construct a new Join object
     *
     * @param type The type of join
     */
    Join::Join(JoinType type) : _type(type) {}

    /**
     * @brief Convert the Join object to an SQL string.
     *
     * @return The SQL string representation of the join.
     */
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

    /**
     * @brief Add a new join to the collection.
     *
     * @param join The join to add.
     * @return Joins& A reference to this Joins object.
     */
    Joins& Joins::add(const Join& join)
    {
        _joins.push_back(join);
        return *this;
    }

    /**
     * @brief Convert the Joins object to an SQL string.
     *
     * @return The SQL string representation of the joins.
     */
    std::string Joins::toSQL() const
    {
        std::string sql;
        for (const auto& join_ : _joins)
        {
            sql += " ";
            sql += join_.toSQL();
        }
        return sql;
    }

}   // namespace orm