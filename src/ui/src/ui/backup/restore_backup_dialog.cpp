#include "ui/backup/restore_backup_dialog.hpp"

#include <QAbstractTableModel>
#include <QDialogButtonBox>
#include <QFileInfo>
#include <QHeaderView>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QTableView>
#include <QVBoxLayout>

#include "common/qt_helpers.hpp"
#include "common/timestamp.hpp"
#include "config/constants/constants.hpp"

namespace ui
{

    namespace
    {
        /**
         * @brief Table model backing the backup list.
         *
         * Columns: Date/Time | File Size
         */
        class BackupTableModel : public QAbstractTableModel
        {
           public:
            explicit BackupTableModel(
                const std::vector<std::string>& backups,
                QObject*                        parent = nullptr
            )
                : QAbstractTableModel{parent}, _backups{backups}
            {
            }

            [[nodiscard]]
            int rowCount(const QModelIndex& parent) const override
            {
                return parent.isValid() ? 0 : static_cast<int>(_backups.size());
            }

            [[nodiscard]]
            int columnCount(const QModelIndex& parent) const override
            {
                return parent.isValid() ? 0 : 2;
            }

            [[nodiscard]]
            QVariant data(const QModelIndex& index, int role) const override
            {
                if (!index.isValid() || index.row() >= rowCount({}))
                    return {};

                const auto& pathStr =
                    _backups[static_cast<std::size_t>(index.row())];
                const auto& path = std::filesystem::path{pathStr};

                if (role == Qt::DisplayRole)
                {
                    if (index.column() == 0)
                    {
                        const auto timeStamp = Timestamp::fromFileSafe(
                            path.filename().string(),
                            Constants::getFilePrefix(),
                            Constants::getDatabaseFileExtension()
                        );

                        if (!timeStamp.has_value())
                            return QString::fromStdString(
                                path.filename().string()
                            );

                        return timeStamp.value().toQDateTime();
                    }

                    if (index.column() == 1)
                    {
                        const auto bytes = std::filesystem::file_size(path);
                        return common::toKBString(bytes);
                    }
                }

                if (role == Qt::TextAlignmentRole)
                {
                    if (index.column() == 1)
                        return static_cast<int>(
                            Qt::AlignRight | Qt::AlignVCenter
                        );
                    return static_cast<int>(Qt::AlignLeft | Qt::AlignVCenter);
                }

                return {};
            }

            [[nodiscard]]
            QVariant headerData(
                int             section,
                Qt::Orientation orientation,
                int             role
            ) const override
            {
                if (orientation != Qt::Horizontal || role != Qt::DisplayRole)
                    return {};
                return section == 0 ? "Date / Time" : "File Size";
            }

            [[nodiscard]]
            Qt::ItemFlags flags(const QModelIndex& index) const override
            {
                if (!index.isValid())
                    return Qt::NoItemFlags;
                return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
            }

           private:
            const std::vector<std::string>& _backups;
        };

    }   // namespace

    /**
     * @brief Construct a RestoreBackupDialog.
     *
     * @param backups Backup paths sorted newest first
     * @param parent  Optional parent widget
     */
    RestoreBackupDialog::RestoreBackupDialog(
        std::vector<std::string> backups,
        QWidget*                 parent
    )
        : Dialog{parent}, _backups{std::move(backups)}
    {
        setWindowTitle("Restore from Backup");

        auto* layout = common::makeQChild<QVBoxLayout>(this);

        auto* label = common::makeQChild<QLabel>(
            "Select a backup to restore. The current database will be "
            "replaced and all stores will be reloaded.",
            this
        );
        label->setWordWrap(true);
        layout->addWidget(label);

        _tableView = common::makeQChild<QTableView>(this);
        auto* model =
            common::makeQChild<BackupTableModel>(_backups, _tableView);
        _tableView->setModel(model);
        _tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
        _tableView->setSelectionMode(QAbstractItemView::SingleSelection);
        _tableView->horizontalHeader()->setSectionResizeMode(
            0,
            QHeaderView::Stretch
        );
        _tableView->horizontalHeader()->setSectionResizeMode(
            1,
            QHeaderView::ResizeToContents
        );
        _tableView->verticalHeader()->setVisible(false);
        _tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
        if (!_backups.empty())
            _tableView->selectRow(0);
        layout->addWidget(_tableView);

        auto* buttons = common::makeQChild<QDialogButtonBox>(
            QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
            this
        );
        buttons->button(QDialogButtonBox::Ok)->setText("Restore");
        connect(
            buttons,
            &QDialogButtonBox::accepted,
            this,
            [this]()
            {
                const auto result = QMessageBox::warning(
                    this,
                    "Confirm Restore",
                    "Restoring a backup will discard all unsaved changes and "
                    "replace the current database. Are you sure?",
                    QMessageBox::Ok | QMessageBox::Cancel,
                    QMessageBox::Cancel
                );
                if (result == QMessageBox::Ok)
                    accept();
            }
        );
        connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
        layout->addWidget(buttons);
    }

    /**
     * @brief Return the selected backup path, or std::nullopt if the dialog
     * was cancelled or no row was selected.
     *
     * @return std::optional<std::string>
     */
    std::optional<std::string> RestoreBackupDialog::selectedBackup() const
    {
        const auto* selection = _tableView->selectionModel();
        if (selection == nullptr || !selection->hasSelection())
            return std::nullopt;

        const auto row = selection->selectedRows().first().row();
        if (row < 0 || static_cast<std::size_t>(row) >= _backups.size())
            return std::nullopt;

        return _backups[static_cast<std::size_t>(row)];
    }

}   // namespace ui
