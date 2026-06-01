#ifndef __UI__INCLUDE__UI__POSITION__POSITION_TABLE_MODEL_HPP__
#define __UI__INCLUDE__UI__POSITION__POSITION_TABLE_MODEL_HPP__

#include <QAbstractTableModel>
#include <QString>
#include <vector>

#include "ui/position/position_columns.hpp"

namespace drafts
{
    class PositionDetailDraft;   // Forward declaration
}   // namespace drafts

namespace finance
{
    class PriceCache;   // Forward declaration
}   // namespace finance

namespace ui
{

    /**
     * @brief Model for displaying position data in a table view
     *
     */
    class PositionTableModel : public QAbstractTableModel
    {
        Q_OBJECT
        /// Vector of position drafts
        std::vector<drafts::PositionDetailDraft> _positions;
        /// Pointer to the price cache
        const finance::PriceCache* _priceCache;

       public:
        explicit PositionTableModel(
            const finance::PriceCache* priceCache = nullptr,
            QObject*                   parent     = nullptr
        );

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
            const std::vector<drafts::PositionDetailDraft>& positions
        );

       private:
        [[nodiscard]]
        QVariant _priceDisplay(
            const drafts::PositionDetailDraft& pos,
            PositionColumns                    col
        ) const;

        [[nodiscard]]
        static QString _columnLabel(int index);
    };

}   // namespace ui

#endif   // __UI__INCLUDE__UI__POSITION__POSITION_TABLE_MODEL_HPP__