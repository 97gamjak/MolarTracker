#include "ui/migration/migration_history_dialog.hpp"

#include <QAbstractTableModel>
#include <QDialogButtonBox>
#include <QHeaderView>
#include <QLabel>
#include <QTableView>
#include <QVBoxLayout>

#include "common/qt_helpers.hpp"

namespace ui
{

    namespace
    {
        /**
         * @brief Table model backing the migration history list.
         *
         * Columns: Version | Release | Applied At
         */
        class MigrationHistoryTableModel : public QAbstractTableModel
        {
           public:
            explicit MigrationHistoryTableModel(
                const std::vector<MigrationHistoryEntry>& entries,
                QObject*                                  parent = nullptr
            )
                : QAbstractTableModel{parent}, _entries{entries}
            {
            }

            [[nodiscard]]
            int rowCount(const QModelIndex& parent) const override
            {
                return parent.isValid() ? 0 : static_cast<int>(_entries.size());
            }

            [[nodiscard]]
            int columnCount(const QModelIndex& parent) const override
            {
                return parent.isValid() ? 0 : 3;
            }

            [[nodiscard]]
            QVariant data(const QModelIndex& index, int role) const override
            {
                if (!index.isValid() || index.row() >= rowCount({}))
                    return {};

                const auto& entry =
                    _entries[static_cast<std::size_t>(index.row())];

                if (role == Qt::DisplayRole)
                {
                    switch (index.column())
                    {
                        case 0:
                            return QString("%1 → %2")
                                .arg(entry.fromVersion)
                                .arg(entry.toVersion);
                        case 1:
                            return QString::fromStdString(entry.releaseVersion);
                        case 2:
                            return QString::fromStdString(entry.appliedAt);
                        default:
                            return {};
                    }
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

                switch (section)
                {
                    case 0:
                        return "Version";
                    case 1:
                        return "Release";
                    case 2:
                        return "Applied At";
                    default:
                        return {};
                }
            }

            [[nodiscard]]
            Qt::ItemFlags flags(const QModelIndex& index) const override
            {
                if (!index.isValid())
                    return Qt::NoItemFlags;
                return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
            }

           private:
            const std::vector<MigrationHistoryEntry>& _entries;
        };

    }   // namespace

    /**
     * @brief Construct a MigrationHistoryDialog.
     *
     * @param entries Recorded migration log entries, oldest first
     * @param currentDbVersion The database's current schema version
     * @param targetDbVersion The schema version this app version migrates to
     * @param parent Parent widget
     */
    MigrationHistoryDialog::MigrationHistoryDialog(
        std::vector<MigrationHistoryEntry> entries,
        std::int64_t                       currentDbVersion,
        std::int64_t                       targetDbVersion,
        QWidget*                           parent
    )
        : Dialog{parent}, _entries{std::move(entries)}
    {
        setWindowTitle("Migration History");
        constexpr auto size = QSize(500, 400);
        resize(size);

        auto* layout = common::makeQChild<QVBoxLayout>(this);

        auto* versionLabel = common::makeQChild<QLabel>(
            QString("Database schema version: %1 of %2")
                .arg(currentDbVersion)
                .arg(targetDbVersion),
            this
        );
        layout->addWidget(versionLabel);

        auto* noteLabel = common::makeQChild<QLabel>(
            "Only migrations applied after this history feature shipped are "
            "recorded below.",
            this
        );
        noteLabel->setWordWrap(true);
        layout->addWidget(noteLabel);

        _tableView  = common::makeQChild<QTableView>(this);
        auto* model = common::makeQChild<MigrationHistoryTableModel>(
            _entries,
            _tableView
        );
        _tableView->setModel(model);
        _tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
        _tableView->setSelectionMode(QAbstractItemView::SingleSelection);
        _tableView->horizontalHeader()->setSectionResizeMode(
            0,
            QHeaderView::ResizeToContents
        );
        _tableView->horizontalHeader()->setSectionResizeMode(
            1,
            QHeaderView::ResizeToContents
        );
        _tableView->horizontalHeader()->setSectionResizeMode(
            2,
            QHeaderView::Stretch
        );
        _tableView->verticalHeader()->setVisible(false);
        _tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
        layout->addWidget(_tableView);

        auto* buttons =
            common::makeQChild<QDialogButtonBox>(QDialogButtonBox::Close, this);
        connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
        connect(buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
        layout->addWidget(buttons);
    }

}   // namespace ui
