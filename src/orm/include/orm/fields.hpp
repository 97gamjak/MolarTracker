#ifndef __ORM__INCLUDE__ORM__FIELDS_HPP__
#define __ORM__INCLUDE__ORM__FIELDS_HPP__

#include <vector>

#include "orm/type_traits.hpp"
#include "where_expr.hpp"

namespace orm
{
    template <db_model Model>
    std::vector<std::string> getDDl();

    template <db_model Model>
    std::vector<std::string> getFKConstraints();

    template <db_model Model>
    std::vector<std::string> getColumnNames();

    template <db_model Model>
    std::string getColumnNames(const std::string& delimiter);

    template <db_model Model>
    WhereExpr getPkWhere(const Model& model);

    template <db_model Model>
    std::size_t getNumberOfPkFields();

    template <db_model Model>
    std::size_t getNumberOfAutoIncrementPkFields();

    template <db_model Model>
    std::size_t getNumberOfFields();

    template <db_model Model>
    Model loadModelFromStatement(const db::Statement& statement);

    template <db_model... Models>
    std::tuple<Models...> loadTupleFromStatement(
        const db::Statement& statement
    );

    template <db_model... Models>
    std::string getSelection();

}   // namespace orm

#ifndef __ORM__INCLUDE__ORM__FIELDS_TPP__
#include "fields.tpp"
#endif

#endif   // __ORM__INCLUDE__ORM__FIELDS_HPP__