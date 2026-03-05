#ifndef __ORM__CRUD__CRUD_DETAIL_TPP__
#define __ORM__CRUD__CRUD_DETAIL_TPP__

#include "crud_detail.hpp"
#include "orm/fields.hpp"

namespace orm
{
    /**
     * @brief Create a table for the specified model if it doesn't exist
     *
     * @tparam Model
     * @param database
     */
    template <typename Model, typename Group, std::size_t... I>
    void appendUniqueGroupImpl(
        std::string& sql,
        std::index_sequence<I...> /*dummy*/
    )
    {
        sql += ", UNIQUE (";

        bool first = true;

        auto append_column = [&](auto member)
        {
            if (!first)
                sql += ", ";
            first = false;

            sql += std::string((Model{}.*member).getColumnName().view());
        };

        auto const& members = Group::members;

        (std::apply(
            [&](auto... member) { (append_column(member), ...); },
            members
        ));

        sql += ")";
    }

    /**
     * @brief Append SQL for a unique group to the provided SQL string
     *
     * @tparam Model
     * @tparam Group
     * @param sql
     */
    template <typename Model, typename Group>
    void appendUniqueGroup(std::string& sql)
    {
        constexpr auto size = std::tuple_size_v<decltype(Group::members)>;

        appendUniqueGroupImpl<Model, Group>(sql, index_seq<size>{});
    }

    /**
     * @brief Append SQL for all unique groups of a model
     *
     * @tparam Model
     * @tparam Groups
     * @tparam I
     * @param sql
     * @param groups
     * @param index_sequence
     */
    template <typename Model, typename Groups, std::size_t... I>
    void appendAllUniqueGroups(
        std::string& sql,
        Groups /*groups*/,
        std::index_sequence<I...> /*dummy*/
    )
    {
        (appendUniqueGroup<Model, std::tuple_element_t<I, Groups>>(sql), ...);
    }

    /**
     * @brief Get column names from a range of FieldView based on constraints
     *
     * @tparam FieldRange
     * @param fields
     * @param constraint
     * @param mode
     * @return std::vector<std::string>
     */
    template <typename FieldRange>
    std::vector<std::string> getColumnNames(const FieldRange& fields)
    {
        return getColumnNames(
            fields,
            static_cast<ORMConstraint>(0),
            ORMConstraintMode::All   // here we select everything
        );
    }

    /**
     * @brief Get column names from a range of FieldView based on constraints
     *
     * @tparam FieldRange
     * @param fields
     * @param constraint
     * @param mode
     * @return std::vector<std::string>
     */
    template <typename FieldRange>
    std::vector<std::string> getColumnNames(
        const FieldRange& fields,
        ORMConstraint     constraint,
        ORMConstraintMode mode
    )
    {
        return getColumnNames(fields, constraint, mode, "");
    }

    /**
     * @brief Get column names with suffix from a range of FieldView based on
     * constraints
     *
     * @tparam FieldRange
     * @param fields
     * @param constraint
     * @param mode
     * @param suffix
     * @return std::vector<std::string>
     */
    template <typename FieldRange>
    std::vector<std::string> getColumnNames(
        const FieldRange&  fields,
        ORMConstraint      constraint,
        ORMConstraintMode  mode,
        const std::string& suffix
    )
    {
        std::vector<std::string> sqlTexts;

        for (const auto& field : fields)
        {
            if (mode == ORMConstraintMode::Only)
            {
                if ((field.getConstraints() & constraint) != constraint)
                    continue;
            }
            else if (mode == ORMConstraintMode::Not)
            {
                if ((field.getConstraints() & constraint) == constraint)
                    continue;
            }

            sqlTexts.push_back(std::string{field.getColumnName()} + suffix);
        }

        return sqlTexts;
    }

    /**
     * @brief Bind field values to a statement based on constraints
     *
     * @tparam FieldRange
     * @param statement
     * @param fields
     * @param constraint
     * @param mode
     */
    template <typename FieldRange>
    void bindFieldsToStatement(
        db::Statement&    statement,
        const FieldRange& fields,
        ORMConstraint     constraint,
        ORMConstraintMode mode
    )
    {
        int index = 1;

        for (const auto& field : fields)
        {
            if (mode == ORMConstraintMode::Only)
            {
                if ((field.getConstraints() & constraint) == constraint)
                    continue;
            }
            else if (mode == ORMConstraintMode::Not)
            {
                if ((field.getConstraints() & constraint) != constraint)
                    continue;
            }

            field.bind(statement, index);
            ++index;
        }
    }

    /**
     * @brief Load a model instance from the current row of a statement
     *
     * @tparam Model
     * @param statement
     * @return Model
     */
    template <typename Model>
    Model loadModelFromStatement(db::Statement const& statement)
    {
        Model      loadedModel{};
        auto const loadedFieldViews = orm::fields(loadedModel);

        int col = 0;

        for (FieldView const& field : loadedFieldViews)
        {
            field.readFrom(statement, col);
            ++col;
        }

        return loadedModel;
    }

    /**
     * @brief Get the number of primary key fields in a model
     *
     * @tparam Model
     * @return std::size_t
     */
    template <typename Model>
    std::size_t getNumberOfPkFields()
    {
        std::size_t count = 0;

        for (const auto& field : orm::fields<Model>())
        {
            if (field.isPk())
                ++count;
        }

        return count;
    }

}   // namespace orm

#endif   // __ORM__CRUD__CRUD_DETAIL_TPP__