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

#include "utils/qt_helpers.hpp"

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
                const std::vector<std::filesystem::path>& backups,
                QObject*                                  parent = nullptr
            )
                : QAbstractTableModel{parent}, _backups{backups}
            {
            }

            int rowCount(const QModelIndex& parent) const override
            {
                return parent.isValid() ? 0 : static_cast<int>(_backups.size());
            }

            int columnCount(const QModelIndex& parent) const override
            {
                return parent.isValid() ? 0 : 2;
            }

            QVariant data(const QModelIndex& index, int role) const override
            {
                if (!index.isValid() || index.row() >= rowCount({}))
                    return {};

                const auto& path =
                    _backups[static_cast<std::size_t>(index.row())];

                if (role == Qt::DisplayRole)
                {
                    if (index.column() == 0)
                    {
                        // Extract timestamp from stem:
                        // "molartracker_YYYYMMDD_HHMMSS"
                        const auto            stem = path.stem().string();
                        constexpr std::size_t prefixLen =
                            sizeof("molartracker_") - 1;
                        if (stem.size() >= prefixLen + 15)
                        {
                            const auto ts = stem.substr(prefixLen);
                            // YYYYMMDD_HHMMSS → YYYY-MM-DD HH:MM:SS
                            return QString{"%1-%2-%3 %4:%5:%6"}
                                .arg(QString::fromStdString(ts.substr(0, 4)))
                                .arg(QString::fromStdString(ts.substr(4, 2)))
                                .arg(QString::fromStdString(ts.substr(6, 2)))
                                .arg(QString::fromStdString(ts.substr(9, 2)))
                                .arg(QString::fromStdString(ts.substr(11, 2)))
                                .arg(QString::fromStdString(ts.substr(13, 2)));
                        }
                        return QString::fromStdString(path.filename().string());
                    }

                    if (index.column() == 1)
                    {
                        const auto bytes =
                            static_cast<qint64>(std::filesystem::file_size(path)
                            );
                        return QString{"%1 KB"}.arg((bytes + 1023) / 1024);
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

            Qt::ItemFlags flags(const QModelIndex& index) const override
            {
                if (!index.isValid())
                    return Qt::NoItemFlags;
                return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
            }

           private:
            const std::vector<std::filesystem::path>& _backups;
        };

    }   // namespace

    /**
     * @brief Construct a RestoreBackupDialog.
     *
     * @param backups Backup paths sorted newest first
     * @param parent  Optional parent widget
     */
    RestoreBackupDialog::RestoreBackupDialog(
        std::vector<std::filesystem::path> backups,
        QWidget*                           parent
    )
        : Dialog{parent}, _backups{std::move(backups)}
    {
        setWindowTitle("Restore from Backup");
        setMinimumSize(500, 300);

        auto* layout = utils::makeQChild<QVBoxLayout>(this);

        auto* label = utils::makeQChild<QLabel>(
            "Select a backup to restore. The current database will be "
            "replaced and all stores will be reloaded.",
            this
        );
        label->setWordWrap(true);
        layout->addWidget(label);

        _tableView  = utils::makeQChild<QTableView>(this);
        auto* model = utils::makeQChild<BackupTableModel>(_backups, _tableView);
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

        auto* buttons = utils::makeQChild<QDialogButtonBox>(
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
     * @return std::optional<std::filesystem::path>
     */
    std::optional<std::filesystem::path> RestoreBackupDialog::selectedBackup(
    ) const
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
