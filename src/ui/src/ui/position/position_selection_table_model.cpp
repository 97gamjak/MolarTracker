#include "ui/position/position_selection_table_model.hpp"

#include <QColor>
#include <QDateTime>

#include "drafts/position_draft.hpp"

namespace ui
{
    PositionSelectionTableModel::PositionSelectionTableModel(
        const std::vector<drafts::PositionDraft>& positions,
        QObject*                                  parent
    )
        : QAbstractTableModel{parent}, _positions{positions}
    {
    }

    int PositionSelectionTableModel::rowCount(const QModelIndex& parent) const
    {
        if (parent.isValid())
            return 0;
        return static_cast<int>(_positions.size());
    }

    int PositionSelectionTableModel::columnCount(
        const QModelIndex& parent
    ) const
    {
        if (parent.isValid())
            return 0;

        return static_cast<int>(PositionSelectionColumnsMeta::size);
    }

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

    const drafts::PositionDraft& PositionSelectionTableModel::positionAt(
        int row
    ) const
    {
        return _positions[static_cast<std::size_t>(row)];
    }

    QString PositionSelectionTableModel::_columnLabel(int index)
    {
        const auto col = static_cast<PositionSelectionColumns>(index);

        return QString::fromStdString(
            PositionSelectionColumnsMeta::toString(col)
        );
    }
}   // namespace ui