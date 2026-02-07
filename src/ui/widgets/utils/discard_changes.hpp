#ifndef __UI__UTILS__DISCARD_CHANGES_HPP__
#define __UI__UTILS__DISCARD_CHANGES_HPP__

#include <QMessageBox>

class QWidget;

namespace ui
{
    /**
     * @brief Show a confirmation dialog to discard changes
     *
     * @param parent The parent widget for the dialog
     * @return QMessageBox::StandardButton The button clicked by the user
     */
    QMessageBox::StandardButton askDiscardChanges(QWidget* parent);

}   // namespace ui

#endif   // __UI__UTILS__DISCARD_CHANGES_HPP__