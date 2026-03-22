#ifndef __ORM__CONSTRAINTS_HPP__
#define __ORM__CONSTRAINTS_HPP__

#include <cstdint>
#include <mstd/enum.hpp>

#include "type_traits.hpp"

namespace orm
{
    /**
     * @brief Struct representing the cascade delete behavior for a foreign key
     * constraint
     *
     */
    struct CascadeDelete
    {
    };

    /**
     * @brief Struct representing the restrict delete behavior for a foreign key
     * constraint
     *
     */
    struct RestrictDelete
    {
    };

    /**
     * @brief Concept for valid deletion behaviors for foreign key constraints
     *
     * @tparam T
     */
    template <typename T>
    concept DeletionType =
        std::same_as<T, CascadeDelete> || std::same_as<T, RestrictDelete>;

    // NOLINTBEGIN
#define ORM_CONSTRAINT_LIST(X) \
    X(None, 0x00)              \
    X(PrimaryKey, 0x01)        \
    X(AutoIncrement, 0x02)     \
    X(Unique, 0x04)            \
    X(NotNull, 0x08)           \
    X(Nullable, 0x10)          \
    X(ForeignKey, 0x20)

    // cppcheck-suppress syntaxError
    MSTD_ENUM_BITFLAG(ORMConstraint, uint64_t, ORM_CONSTRAINT_LIST);
    // NOLINTEND

    /**
     * @brief struct representing the primary key constraint for a field
     *
     */
    struct primary_key_t
    {
        /// Compile-time constant representing the primary key constraint
        static constexpr ORMConstraint value = ORMConstraint::PrimaryKey;
    };

    /**
     * @brief struct representing the foreign key constraint for a field
     *
     * @tparam DeletionType
     * @tparam TableType
     * @tparam FieldType
     */
    template <DeletionType T, typename TableType, typename FieldType>
    struct foreign_key_t
    {
        /// Compile-time constant representing the foreign key constraint
        static constexpr ORMConstraint value = ORMConstraint::ForeignKey;

        /// Type aliases for the deletion behavior
        using DeletionBehavior = T;
        /// Type alias for the referenced table
        using Table = TableType;
        /// Type alias for the referenced field
        using Field = FieldType;
    };

    /**
     * @brief struct representing the auto-increment constraint for a field
     *
     */
    struct auto_increment_t
    {
        /// Compile-time constant representing the auto-increment constraint
        static constexpr ORMConstraint value = ORMConstraint::AutoIncrement;
    };

    /**
     * @brief struct representing the unique constraint for a field
     *
     */
    struct unique_t
    {
        /// Compile-time constant representing the unique constraint
        static constexpr ORMConstraint value = ORMConstraint::Unique;
    };

    /**
     * @brief struct representing the not-null constraint for a field
     *
     */
    struct not_null_t
    {
        /// Compile-time constant representing the not-null constraint
        static constexpr ORMConstraint value = ORMConstraint::NotNull;
    };

    /**
     * @brief struct representing the nullable constraint for a field
     *
     */
    struct nullable_t
    {
        /// Compile-time constant representing the nullable constraint
        static constexpr ORMConstraint value = ORMConstraint::Nullable;
    };

    /**
     * @brief struct representing the requirement for a paired insert for a
     * model, this is used to indicate that a model cannot be inserted on its
     * own and requires a corresponding insert of another model (e.g., for 1:1
     * relationships)
     *
     */
    struct requires_paired_insert_t
    {
    };

    /**
     * @brief Concept for valid insert policies for models, this is used to
     * determine if a model is freely insertable (i.e., does not require a
     * paired insert)
     *
     * @tparam T
     */
    template <db_model Model, typename Default, typename = void>
    struct insert_policy_or
    {
        /// Type alias for the insert policy of the model, this will be the
        /// Default type if the model does not have a nested insert_policy type
        using type = Default;
    };

    /**
     * @brief Helper struct to determine the insert policy of a model, this
     * checks if the model has a nested type named insert_policy, and if so,
     * uses that as the insert policy, otherwise it defaults to void
     *
     */
    template <db_model Model, typename Default>
    struct insert_policy_or<
        Model,
        Default,
        std::void_t<typename Model::insert_policy>>
    {
        /// Type alias for the insert policy of the model, this will be the type
        /// of the nested insert_policy type if it exists, otherwise it will be
        /// the Default type
        using type = typename Model::insert_policy;
    };

    /**
     * @brief Type trait to determine if a model is freely insertable (i.e.,
     * does not require a paired insert)
     *
     * @tparam Model
     */
    template <db_model Model>
    struct insert_policy
    {
        /// Type alias for the insert policy of the model
        using type = typename insert_policy_or<Model, void>::type;
    };

    /**
     * @brief Variable template to determine if a model is freely insertable
     * (i.e., does not require a paired insert)
     *
     * @tparam Model
     */
    template <db_model Model>
    constexpr bool is_freely_insertable_v = !std::same_as<
        typename insert_policy<Model>::type,
        requires_paired_insert_t>;

}   // namespace orm

#endif   // __ORM__CONSTRAINTS_HPP__