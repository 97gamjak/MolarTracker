#ifndef __UI__INCLUDE__UI__POSITION__POSITION_SELECTION_TABLE_MODEL_HPP__
#define __UI__INCLUDE__UI__POSITION__POSITION_SELECTION_TABLE_MODEL_HPP__

#include <QAbstractTableModel>
#include <cstdint>
#include <mstd/enum.hpp>
#include <optional>
#include <vector>

class Timestamp;   // Forward declaration

namespace drafts
{
    class PositionDraft;   // Forward declaration
}   // namespace drafts

namespace ui
{
// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define POSITION_SELECTION_COLUMN_LIST(X) \
    X(Ticker)                             \
    X(Name)                               \
    X(OpenedAt)

    MSTD_ENUM(
        PositionSelectionColumns,
        std::uint8_t,
        POSITION_SELECTION_COLUMN_LIST
    );

    /**
     * @brief Table model for selecting a position
     *
     */
    class PositionSelectionTableModel : public QAbstractTableModel
    {
        Q_OBJECT

       private:
        /// The list of position drafts
        std::vector<drafts::PositionDraft> _positions;

       public:
        explicit PositionSelectionTableModel(
            const std::vector<drafts::PositionDraft>& positions,
            QObject*                                  parent = nullptr
        );

        [[nodiscard]] int rowCount(const QModelIndex& parent) const override;
        [[nodiscard]] int columnCount(const QModelIndex& parent) const override;

        [[nodiscard]] QVariant data(
            const QModelIndex& index,
            int                role
        ) const override;

        [[nodiscard]] QVariant headerData(
            int             section,
            Qt::Orientation orientation,
            int             role
        ) const override;

        [[nodiscard]] std::optional<drafts::PositionDraft> positionAt(
            int row
        ) const;

       private:
        [[nodiscard]] static QString _columnLabel(int index);
    };

}   // namespace ui

#endif   // __UI__INCLUDE__UI__POSITION__POSITION_SELECTION_TABLE_MODEL_HPP__