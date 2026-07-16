#ifndef __UI__INCLUDE__UI__BACKUP__RESTORE_BACKUP_DIALOG_HPP__
#define __UI__INCLUDE__UI__BACKUP__RESTORE_BACKUP_DIALOG_HPP__

#include <filesystem>
#include <optional>
#include <vector>

#include "ui/base/dialog.hpp"

class QTableView;            // Forward declaration
class QAbstractTableModel;   // Forward declaration

namespace ui
{

    /**
     * @brief Dialog that lists available database backups and lets the user
     * select one to restore from.
     *
     * After `exec()` returns `QDialog::Accepted`, check `selectedBackup()` to
     * obtain the chosen file path. The actual close/copy/reopen is performed by
     * the caller (SettingsMenuController → MainController).
     */
    class RestoreBackupDialog : public Dialog
    {
        Q_OBJECT

       public:
        explicit RestoreBackupDialog(
            std::vector<std::string> backups,
            QWidget*                 parent = nullptr
        );

        [[nodiscard]] std::optional<std::string> selectedBackup() const;

       private:
        /// The list of backup paths (sorted newest first)
        std::vector<std::string> _backups;
        /// The table view that displays the backups
        QTableView* _tableView{nullptr};
    };

}   // namespace ui

#endif   // __UI__INCLUDE__UI__BACKUP__RESTORE_BACKUP_DIALOG_HPP__
