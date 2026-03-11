#ifndef __ORM__JOIN_HPP__
#define __ORM__JOIN_HPP__

#include <string>
#include <utility>
#include <vector>

namespace orm
{
    class Join
    {
       private:
        friend class Joins;

        /// the name of the base table (the one being joined from)
        std::string _baseTable;

        /// the name of the table to join with
        std::string _joinTable;

        /// the fields to join on, e.g. (this_table.field, other_table.field)
        std::vector<std::pair<std::string, std::string>> _fields;

       public:
        explicit Join(
            std::string                                      baseTable,
            std::string                                      joinTable,
            std::vector<std::pair<std::string, std::string>> fields
        );
        explicit Join(
            std::string                         baseTable,
            std::string                         joinTable,
            std::pair<std::string, std::string> fieldPair
        );

        [[nodiscard]] std::string getDBOperations() const;
    };

    class Joins
    {
       private:
        std::vector<Join> _joins;

       public:
        Joins() = default;

        void addJoin(const Join& join);

        [[nodiscard]] std::string getDBOperations(
            const std::string& baseTable
        ) const;
    };

}   // namespace orm

#endif   // __ORM__JOIN_HPP__