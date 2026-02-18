#include "warnings.hpp"

#include "logging/log_manager.hpp"
#include "logging/log_object.hpp"

namespace ui
{
    /**
     * @brief Show a warning message box with the given title and log the
     * message
     *
     * @param title Title of the warning message box
     * @param logObject LogObject containing the warning message and metadata
     */
    void showWarningMessageBox(
        const std::string& title,
        LogObject          logObject,
        QWidget*           parent
    )
    {
        // ensure the log object is logged as a warning
        logObject.level = LogLevel::Warning;

        LogManager::getInstance().log(logObject);

        QMessageBox msgBox(parent);
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setWindowTitle(QString::fromStdString(title));
        msgBox.setText(QString::fromStdString(logObject.message));
        msgBox.exec();
    }
}   // namespace ui