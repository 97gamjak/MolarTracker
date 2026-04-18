#ifndef __UI__INCLUDE__UI__BASE__DIALOG_HPP__
#define __UI__INCLUDE__UI__BASE__DIALOG_HPP__

#include <QDialog>

namespace ui
{
    /**
     * @brief Base class for dialogs in the application, providing common
     * functionality and styling
     *
     */
    class Dialog : public QDialog
    {
        Q_OBJECT

       public:
        explicit Dialog(QWidget* parent);
    };
}   // namespace ui

#endif   // __UI__INCLUDE__UI__BASE__DIALOG_HPP__
