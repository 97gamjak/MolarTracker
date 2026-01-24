#ifndef __UI__EXCEPTIONS__EXCEPTION_DIALOG_HPP__
#define __UI__EXCEPTIONS__EXCEPTION_DIALOG_HPP__

#include <QDialog>
#include <QString>

namespace ui
{

    class ExceptionDialog final : public QDialog
    {
        Q_OBJECT

       public:
        static void showFatal(const QString& title, const QString& details);

       private:
        explicit ExceptionDialog(
            const QString& title,
            const QString& details,
            QWidget*       parent = nullptr
        );
    };

}   // namespace ui

#endif   // __UI__EXCEPTIONS__EXCEPTION_DIALOG_HPP__