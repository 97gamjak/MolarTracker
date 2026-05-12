#ifndef __UI__INCLUDE__UI__POSITION__POSITION_SELECTION_DIALOG_HPP__
#define __UI__INCLUDE__UI__POSITION__POSITION_SELECTION_DIALOG_HPP__

#include <optional>

#include "drafts/position_draft.hpp"
#include "ui/base/dialog.hpp"

class QTableView;
class QPushButton;

namespace ui
{
    class PositionSelectionTableModel;

    class PositionSelectionDialog : public Dialog
    {
        Q_OBJECT

       private:
        PositionSelectionTableModel* _model;
        QTableView*                  _tableView;
        QPushButton*                 _selectBtn;
        QPushButton*                 _createBtn;
        QPushButton*                 _cancelBtn;

        std::optional<drafts::PositionDraft> _selectedPosition;

       public:
        explicit PositionSelectionDialog(
            const std::vector<drafts::PositionDraft>& positions,
            QWidget*                                  parent = nullptr
        );

        [[nodiscard]]
        std::optional<drafts::PositionDraft> selectedPosition() const;

       private:
        void _onSelectionChanged();
        void _onSelectClicked();
        void _onCreateClicked();
    };
}   // namespace ui

#endif   // __UI__INCLUDE__UI__POSITION__POSITION_SELECTION_DIALOG_HPP__