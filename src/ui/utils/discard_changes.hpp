#ifndef __UI__UTILS__DISCARD_CHANGES_HPP__
#define __UI__UTILS__DISCARD_CHANGES_HPP__

#include <QMessageBox>

class QWidget;

namespace ui
{
    QMessageBox::StandardButton askDiscardChanges(QWidget* parent);

}   // namespace ui

#endif   // __UI__UTILS__DISCARD_CHANGES_HPP__