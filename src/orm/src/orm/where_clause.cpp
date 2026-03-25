#include "orm/where_clause.hpp"

namespace orm
{

    /**
     * @brief Get the SQL operations for all where clauses, e.g.
     * 'WHERE field1 = ? AND field2 > ?'
     *
     * @return std::string
     */
    [[nodiscard]] std::string WhereClauses::getDBOperations() const
    {
        if (_clauses.empty())
            return "";

        std::string result = "WHERE ";

        for (std::size_t i = 0; i < _clauses.size(); ++i)
        {
            if (i > 0)
                result += " AND ";

            result += _clauses[i]->getDBOperations() + " ";
        }

        return result;
    }

    /**
     * @brief Bind the values for all where clauses to the specified statement
     *
     * @param statement
     */
    void WhereClauses::bind(db::Statement& statement) const
    {
        for (std::size_t i = 0; i < _clauses.size(); ++i)
            _clauses[i]->bind(statement, bindIndex(i));
    }

    /**
     * @brief Check if there are no where clauses in this collection
     *
     * @return true
     * @return false
     */
    [[nodiscard]] bool WhereClauses::empty() const { return _clauses.empty(); }

    /**
     * @brief Get the number of where clauses in this collection
     *
     * @return std::size_t
     */
    [[nodiscard]] std::size_t WhereClauses::size() const
    {
        return _clauses.size();
    }

}   // namespace orm