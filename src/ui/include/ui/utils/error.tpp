#ifndef __UI__INCLUDE__UI__UTILS__ERROR_TPP__
#define __UI__INCLUDE__UI__UTILS__ERROR_TPP__

#include "ui/utils/error.hpp"

namespace ui
{
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