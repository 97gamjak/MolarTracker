#ifndef __SQL_MODELS__INCLUDE__SQL_MODELS__POSITION_ROW_HPP__
#define __SQL_MODELS__INCLUDE__SQL_MODELS__POSITION_ROW_HPP__

#include "config/id_types.hpp"
#include "orm/constraints.hpp"
#include "orm/field.hpp"
#include "orm/orm_model.hpp"
#include "orm/where_expr.hpp"
#include "utils/timestamp.hpp"

/**
 * @brief Represents a position in the trading system.
 *
 */
struct PositionRow : public orm::ORMModel<"position">
{
    /// The ID of the position, this is the primary key for the position table
    ORM_FIELD(id, IdField<PositionId>)

    /// The timestamp when the position was opened
    ORM_FIELD(createdAt, Field<"opened_at", Timestamp, orm::not_null_t>)

    /// The timestamp when the position was closed
    ORM_FIELD(closedAt, Field<"closed_at", std::optional<Timestamp>>)

    /// @cond DOXYGEN_IGNORE
    ORM_FIELDS(PositionRow, id, createdAt, closedAt)
    /// @endcond

    [[nodiscard]]
    static orm::WhereExpr IsOpen();
};

#endif   // __SQL_MODELS__INCLUDE__SQL_MODELS__POSITION_ROW_HPP__