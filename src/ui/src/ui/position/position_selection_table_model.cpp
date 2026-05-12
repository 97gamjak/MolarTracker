#include "ui/position/position_selection_table_model.hpp"

#include <QColor>
#include <QDateTime>

#include "drafts/position_draft.hpp"

namespace ui
{
    /**
     * @brief Construct a new Position Selection Table Model:: Position
     * Selection Table Model object
     *
     * @param positions The list of position drafts
     * @param parent The parent object
     */
    PositionSelectionTableModel::PositionSelectionTableModel(
        const std::vector<drafts::PositionDraft>& positions,
        QObject*                                  parent
    )
        : QAbstractTableModel{parent}, _positions{positions}
    {
    }

    /**
     * @brief Get the number of rows in the model
     *
     * @param parent The parent index
     * @return int The number of rows
     */
    int PositionSelectionTableModel::rowCount(const QModelIndex& parent) const
    {
        if (parent.isValid())
            return 0;
        return static_cast<int>(_positions.size());
    }

    /**
     * @brief Get the number of columns in the model
     *
     * @param parent The parent index
     * @return int The number of columns
     */
    int PositionSelectionTableModel::columnCount(
        const QModelIndex& parent
    ) const
    {
        if (parent.isValid())
            return 0;

        return static_cast<int>(PositionSelectionColumnsMeta::size);
    }

    /**
     * @brief Get the data for a specific index
     *
     * @param index The model index
     * @param role The data role
     * @return QVariant The data for the index
     */
    QVariant PositionSelectionTableModel::data(
        const QModelIndex& index,
        int                role
    ) const
    {
        if (!index.isValid() || index.row() >= rowCount({}))
            return {};

        const auto& pos = _positions[static_cast<std::size_t>(index.row())];
        const auto  col = static_cast<PositionSelectionColumns>(index.column());

        switch (role)
        {
            case Qt::DisplayRole:
            {
                switch (col)
                {
                    case ui::PositionSelectionColumns::Ticker:
                        return QString::fromStdString(
                            pos.getStockInfo().getTicker()
                        );
                    case ui::PositionSelectionColumns::Name:
                        return QString::fromStdString(
                            pos.getStockInfo().getShortName()
                        );
                    case ui::PositionSelectionColumns::OpenedAt:
                        return pos.getCreatedAt().toQDateTime().toString();
                }
                return {};
            }

            case Qt::TextAlignmentRole:
            {
                switch (col)
                {
                    case ui::PositionSelectionColumns::OpenedAt:
                        return static_cast<int>(
                            Qt::AlignRight | Qt::AlignVCenter
                        );
                    case ui::PositionSelectionColumns::Name:
                    case ui::PositionSelectionColumns::Ticker:
                        return static_cast<int>(
                            Qt::AlignLeft | Qt::AlignVCenter
                        );
                }
                std::unreachable();
            }

            default:
                return {};
        }
    }

    /**
     * @brief Get the header data for a specific section
     *
     * @param section The section index
     * @param orientation The orientation of the header
     * @param role The data role
     * @return QVariant The header data for the section
     */
    QVariant PositionSelectionTableModel::headerData(
        int             section,
        Qt::Orientation orientation,
        int             role
    ) const
    {
        if (orientation != Qt::Horizontal || role != Qt::DisplayRole)
            return {};
        return _columnLabel(section);
    }

    /**
     * @brief Get the position draft at a specific row
     *
     * @param row The row index
     * @return const drafts::PositionDraft& The position draft at the row
     */
    const drafts::PositionDraft& PositionSelectionTableModel::positionAt(
        int row
    ) const
    {
        return _positions[static_cast<std::size_t>(row)];
    }

    /**
     * @brief Get the column label for a specific index
     *
     * @param index The column index
     * @return QString The column label
     */
    QString PositionSelectionTableModel::_columnLabel(int index)
    {
        const auto col = static_cast<PositionSelectionColumns>(index);

        return QString::fromStdString(
            PositionSelectionColumnsMeta::toString(col)
        );
    }
}   // namespace ui