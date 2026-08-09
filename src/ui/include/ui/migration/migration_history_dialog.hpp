#ifndef __UI__INCLUDE__UI__MIGRATION__MIGRATION_HISTORY_DIALOG_HPP__
#define __UI__INCLUDE__UI__MIGRATION__MIGRATION_HISTORY_DIALOG_HPP__

#include <cstdint>
#include <string>
#include <vector>

#include "ui/base/dialog.hpp"

class QTableView;   // Forward declaration

namespace ui
{

    /**
     * @brief One applied database migration step, for display purposes
     *
     */
    struct MigrationHistoryEntry
    {
        /// The schema version this step was applied from
        std::int64_t fromVersion;
        /// The schema version this step was applied to
        std::int64_t toVersion;
        /// The app release version this step shipped with (e.g. "0.3.0")
        std::string releaseVersion;
        /// When this step was applied, as an ISO-8601 UTC string
        std::string appliedAt;
    };

    /**
     * @brief Read-only dialog listing every recorded database migration
     * step, with when it was applied on this install.
     *
     * Only migration steps applied after this feature shipped are recorded
     * — there is no retroactive record of earlier migrations, since the
     * underlying migration_log table didn't exist yet while they ran.
     */
    class MigrationHistoryDialog : public Dialog
    {
        Q_OBJECT

       public:
        MigrationHistoryDialog(
            std::vector<MigrationHistoryEntry> entries,
            std::int64_t                       currentDbVersion,
            std::int64_t                       targetDbVersion,
            QWidget*                           parent
        );

       private:
        /// The recorded migration log entries, oldest first
        std::vector<MigrationHistoryEntry> _entries;
        /// The table view that displays the entries
        QTableView* _tableView{nullptr};
    };

}   // namespace ui

#endif   // __UI__INCLUDE__UI__MIGRATION__MIGRATION_HISTORY_DIALOG_HPP__
