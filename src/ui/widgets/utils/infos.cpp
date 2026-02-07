#include "infos.hpp"

#include "logging/log_manager.hpp"
#include "logging/log_object.hpp"

namespace ui
{
    /**
     * @brief Show an info message in the status bar and log the message
     *
     * @param logObject LogObject containing the info message and metadata
     * @param statusBar Pointer to the QStatusBar where the message will be
     * shown
     */
    void showInfoStatusBar(LogObject logObject, QStatusBar* statusBar)
    {
        // ensure the log object is logged as an info
        logObject.level = LogLevel::Info;

        LogManager::getInstance().log(logObject);

        if (statusBar)
            statusBar->showMessage(QString::fromStdString(logObject.message));
    }
}   // namespace ui