#ifndef __UI__INCLUDE__UI__POSITION__POSITION_SELECTION_DIALOG_HPP__
#define __UI__INCLUDE__UI__POSITION__POSITION_SELECTION_DIALOG_HPP__

#include <optional>

#include "drafts/position/position_stock_draft.hpp"
#include "ui/base/dialog.hpp"

class QTableView;
class QPushButton;

namespace ui
{
    class PositionSelectionTableModel;

    /**
     * @brief Dialog for selecting a position from a list of available positions
     *
     */
    class PositionSelectionDialog : public Dialog
    {
        Q_OBJECT

       private:
        /// The model for the position selection table
        PositionSelectionTableModel* _model;
        /// The view for the position selection table
        QTableView* _tableView;
        /// The button for selecting a position
        QPushButton* _selectBtn;
        /// The button for creating a new position
        QPushButton* _createBtn;
        /// The button for canceling the selection
        QPushButton* _cancelBtn;

        /// The currently selected position
        std::optional<drafts::PositionStockDetailDraft> _selectedPosition;

       public:
        explicit PositionSelectionDialog(
            const std::vector<drafts::PositionStockDetailDraft>& positions,
            QWidget* parent = nullptr
        );

        [[nodiscard]]
        std::optional<drafts::PositionStockDetailDraft> selectedPosition(
        ) const;

       private:
        void _onSelectionChanged();
        void _onSelectClicked();
        void _onCreateClicked();
    };
}   // namespace ui

#endif   // __UI__INCLUDE__UI__POSITION__POSITION_SELECTION_DIALOG_HPP__