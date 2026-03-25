#include "orm/join.hpp"

#include "orm/orm_exception.hpp"

namespace orm
{
    /**
     * @brief Construct a new Join object
     *
     * @param baseTable The name of the base table (the one being joined from)
     * @param joinTable The name of the table to join with
     * @param fields A vector of pairs of fields to join on, e.g. {("field1",
     * "field2"), ("field3", "field4")}
     */
    Join::Join(
        std::string                                      baseTable,
        std::string                                      joinTable,
        std::vector<std::pair<std::string, std::string>> fields
    )
        : _baseTable(std::move(baseTable)),
          _joinTable(std::move(joinTable)),
          _fields(std::move(fields))
    {
    }

    /**
     * @brief Construct a new Join object with a single field pair
     *
     * @param baseTable The name of the base table (the one being joined from)
     * @param joinTable The name of the table to join with
     * @param fieldPair A pair of fields to join on, e.g. ("field1", "field2")
     */
    Join::Join(
        std::string                         baseTable,
        std::string                         joinTable,
        std::pair<std::string, std::string> fieldPair
    )
        : _baseTable(std::move(baseTable)),
          _joinTable(std::move(joinTable)),
          _fields({std::move(fieldPair)})
    {
    }

    /**
     * @brief Get the SQL operations for this join, e.g. "JOIN other_table ON
     * this_table.field = other_table.field"
     *
     * @tparam Field
     * @return const std::string&
     */
    std::string Join::getDBOperations() const
    {
        std::string sqlText;
        sqlText += "JOIN ";
        sqlText += _joinTable;
        sqlText += " ON ";

        for (std::size_t i = 0; i < _fields.size(); ++i)
        {
            if (i > 0)
                sqlText += " AND ";

            sqlText += _baseTable + "." + _fields[i].first;
            sqlText += " = ";
            sqlText += _joinTable + "." + _fields[i].second;
        }

        return sqlText;
    }

    /**
     * @brief Add a join to the collection of joins
     *
     * @param join
     */
    void Joins::addJoin(const Join& join)
    {
        if (!_joins.empty() && _joins.back()._baseTable != join._baseTable)
        {
            throw ORMError(
                "All joins in a Joins object must have the same base table"
            );
        }

        _joins.push_back(join);
    }

    /**
     * @brief Get the SQL operations for all joins, e.g. "JOIN other_table ON
     * this_table.field = other_table.field JOIN another_table ON
     * this_table.field2 = another_table.field2"
     *
     * @param baseTable The name of the base table (the one being joined from)
     *
     * @return std::string
     */
    std::string Joins::getDBOperations(const std::string& baseTable) const
    {
        // ensure once again that all joins have the same base table
        for (std::size_t i = 1; i < _joins.size(); ++i)
        {
            if (_joins[i]._baseTable != baseTable)
            {
                throw ORMError(
                    "All joins in a Joins object must have the same base table"
                );
            }
        }

        std::string sqlText = "FROM " + baseTable;

        for (const auto& join : _joins)
            sqlText += " " + join.getDBOperations();

        return sqlText;
    }

}   // namespace orm