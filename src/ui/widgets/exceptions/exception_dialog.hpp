#ifndef __UI__WIDGETS__EXCEPTIONS__EXCEPTION_DIALOG_HPP__
#define __UI__WIDGETS__EXCEPTIONS__EXCEPTION_DIALOG_HPP__

#include <QDialog>

namespace logging
{
    class LogObject;   // forward declaration
}   // namespace logging

class QString;   // forward declaration

namespace ui
{
    /**
     * @brief A dialog for displaying exceptions
     */
    class ExceptionDialog final : public QDialog
    {
        Q_OBJECT

       public:
        static void showFatal(
            const QString&     title,
            logging::LogObject logObject,
            QWidget*           parent
        );

        static void showFatal(
            const QString&     title,
            logging::LogObject logObject
        );

       private:
        explicit ExceptionDialog(
            const QString& title,
            const QString& details,
            QWidget*       parent
        );
    };

}   // namespace ui

#endif   // __UI__WIDGETS__EXCEPTIONS__EXCEPTION_DIALOG_HPP__