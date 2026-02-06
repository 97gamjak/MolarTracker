#include "discard_changes.hpp"

#include <QMessageBox>
#include <QWidget>

namespace ui
{
    /**
     * @brief Asks the user whether to discard changes
     *
     * @param parent Parent widget
     * @return QMessageBox::StandardButton User's choice
     */
    QMessageBox::StandardButton askDiscardChanges(QWidget* parent)
    {
        using enum QMessageBox::StandardButton;

        return QMessageBox::question(
            parent,
            "Discard changes?",
            "Are you sure you want to discard your changes?",
            Yes | No,
            No
        );
    }

}   // namespace ui