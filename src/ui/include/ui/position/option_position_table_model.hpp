#ifndef __UI__INCLUDE__UI__POSITION__OPTION_POSITION_TABLE_MODEL_HPP__
#define __UI__INCLUDE__UI__POSITION__OPTION_POSITION_TABLE_MODEL_HPP__

#include <QAbstractTableModel>
#include <QString>
#include <vector>

#include "drafts/position/position_option_draft.hpp"   // TODO; remove this
#include "position_columns.hpp"

namespace drafts
{
    class PositionOptionDetailDraft;   // Forward declaration
}   // namespace drafts

namespace finance
{
    class PriceCache;   // Forward declaration
}   // namespace finance

namespace ui
{

    /**
     * @brief Model for displaying option position data in a table view
     *
     */
    class OptionPositionTableModel : public QAbstractTableModel
    {
        Q_OBJECT
        /// Vector of position drafts
        std::vector<drafts::PositionOptionDetailDraft> _positions;

       public:
        explicit OptionPositionTableModel(QObject* parent = nullptr);

        [[nodiscard]]
        int rowCount(const QModelIndex& parent) const override;

        [[nodiscard]]
        int columnCount(const QModelIndex& parent) const override;

        [[nodiscard]]
        QVariant data(const QModelIndex& index, int role) const override;

        [[nodiscard]]
        QVariant headerData(
            int             section,
            Qt::Orientation orientation,
            int             role
        ) const override;

        void refreshPrices();

        void setPositions(
            const std::vector<drafts::PositionOptionDetailDraft>& positions
        );

       private:
        [[nodiscard]]
        static QString _columnLabel(int index);
    };

}   // namespace ui

#endif   // __UI__INCLUDE__UI__POSITION__OPTION_POSITION_TABLE_MODEL_HPP__