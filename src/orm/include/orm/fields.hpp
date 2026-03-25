#ifndef __ORM__FIELDS_HPP__
#define __ORM__FIELDS_HPP__

#include <vector>

#include "orm/type_traits.hpp"
#include "where_clause.hpp"

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
    WhereClauses getPkWhereClauses(const Model& model);

    template <db_model Model>
    std::size_t getNumberOfPkFields();

    template <db_model Model>
    std::size_t getNumberOfAutoIncrementPkFields();

    template <db_model Model>
    std::size_t getNumberOfFields();

    template <typename Model>
    Model loadModelFromStatement(db::Statement const& statement);

}   // namespace orm

#ifndef __ORM__FIELDS_TPP__
#include "fields.tpp"
#endif

#endif   // __ORM__FIELDS_HPP__