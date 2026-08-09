#ifndef __SQL_MODELS__INCLUDE__SQL_MODELS__MIGRATION_LOG_ROW_HPP__
#define __SQL_MODELS__INCLUDE__SQL_MODELS__MIGRATION_LOG_ROW_HPP__

#include <cstdint>
#include <string>

#include "config/id_types.hpp"
#include "orm/field.hpp"
#include "orm/fixed_string.hpp"
#include "orm/orm_model.hpp"

/**
 * @brief Represents a row in the "migration_log" database table, recording
 * one applied database migration step (from_version -> to_version) along
 * with the app release it shipped with and when it was applied on this
 * install.
 *
 * Only migration steps run after this table was introduced (see
 * MigrationRunner::DB_VERSION history) are recorded — there is no
 * retroactive record of earlier migrations.
 */
struct MigrationLogRow : orm::ORMModel<"migration_log">
{
    /// The id field, this is the primary key of the table and is
    /// auto-incremented
    ORM_FIELD(id, IdField<MigrationLogId>)

    /// The schema version this migration step was applied from
    ORM_FIELD(fromVersion, Field<"from_version", std::int64_t, orm::not_null_t>)

    /// The schema version this migration step was applied to
    ORM_FIELD(toVersion, Field<"to_version", std::int64_t, orm::not_null_t>)

    /// The app release version this migration step shipped with (e.g.
    /// "0.3.0")
    ORM_FIELD(
        releaseVersion,
        Field<"release_version", std::string, orm::not_null_t>
    )

    /// When this migration step was applied, as an ISO-8601 UTC string
    ORM_FIELD(appliedAt, Field<"applied_at", std::string, orm::not_null_t>)

    /// @cond DOXYGEN_IGNORE
    ORM_FIELDS(
        MigrationLogRow,
        id,
        fromVersion,
        toVersion,
        releaseVersion,
        appliedAt
    )
    /// @endcond
};

#endif   // __SQL_MODELS__INCLUDE__SQL_MODELS__MIGRATION_LOG_ROW_HPP__
