#ifndef __SQL_MODELS__INCLUDE__SQL_MODELS__POSITION_ROW_HPP__
#define __SQL_MODELS__INCLUDE__SQL_MODELS__POSITION_ROW_HPP__

#include "config/id_types.hpp"
#include "orm/constraints.hpp"
#include "orm/field.hpp"
#include "orm/orm_model.hpp"
#include "utils/timestamp.hpp"

/**
 * @brief Represents a position in the trading system.
 *
 */
struct PositionRow : public orm::ORMModel<"position">
{
    ORM_FIELD(id, IdField<PositionId>)
    ORM_FIELD(openedAt, Field<"opened_at", Timestamp, orm::not_null_t>)
    ORM_FIELD(closedAt, Field<"closed_at", std::optional<Timestamp>>)

    ORM_FIELDS(PositionRow, id, openedAt, closedAt)
};

#endif   // __SQL_MODELS__INCLUDE__SQL_MODELS__POSITION_ROW_HPP__