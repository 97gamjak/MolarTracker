#include "orm/crud.hpp"

namespace orm
{
    /**
     * @brief Get the SQL statements that have been executed
     *
     * @return const std::vector<std::string>& A reference to the vector of
     * executed SQL statements
     */
    [[nodiscard]] const std::vector<std::string>& Crud::getExecutedSQL() const
    {
        return _sqlExecutions;
    }

    /**
     * @brief Check if a column exists in the database
     *
     * @param database
     * @return true if the column exists, false otherwise
     */
    bool Crud::_columnExists(
        db::Database&      database,
        const std::string& columnName,
        const std::string& tableName
    )
    {
        std::string sql;
        sql += "SELECT COUNT(*) FROM PRAGMA_TABLE_INFO('";
        sql += tableName;
        sql += "') WHERE name = '";
        sql += columnName + "'";

        db::Statement statement = database.prepare(sql);

        _sqlExecutions.push_back(sql);

        if (statement.step() == db::StepResult::RowAvailable)
            return statement.columnInt64(0) > 0;

        throw orm::CrudException("Failed to check if column exists");
    }

}   // namespace orm
