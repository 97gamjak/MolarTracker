#ifndef __UI__INCLUDE__UI__UTILS__ERROR_TPP__
#define __UI__INCLUDE__UI__UTILS__ERROR_TPP__

#include "ui/utils/error.hpp"

namespace ui
{
    /**
     * @brief Show an error dialog for a given error, this method creates and
     * displays an error dialog with the specified error message and optional
     * details. If a custom message is provided, it will be displayed in the
     * dialog, otherwise the error's string representation will be used.
     *
     * @tparam Error The type of the error, which must satisfy the IsError
     * concept.
     * @param error The error object to display in the dialog.
     * @param msg An optional custom message to display in the dialog.
     * @param parent An optional parent widget for the dialog.
     */
    template <typename Error>
    void ErrorDialog::show(
        const Error&                      error,
        const std::optional<std::string>& msg,
        QWidget*                          parent
    )
    {
        const auto title = QString::fromStdString(error.getTypeStr());
        std::optional<QString> details = std::nullopt;
        QString                message;

        if (msg.has_value())
        {
            message = QString::fromStdString(*msg);
            details = QString::fromStdString(error.toString());
        }
        else
        {
            message = QString::fromStdString(error.toString());
        }

        ErrorDialog dlg(title, message, details, parent);
        dlg.exec();
    }
}   // namespace ui

#endif   // __UI__INCLUDE__UI__UTILS__ERROR_TPP__