#include "ui/menu_bar/utilities_menu.hpp"

#include <qapplication.h>
#include <qmainwindow.h>
#include <qnamespace.h>
#include <qpainter.h>
#include <qscreen.h>

#include <QAction>
#include <QFileDialog>
#include <QMenu>
#include <QMenuBar>
#include <format>

#include "common/qt_helpers.hpp"
#include "common/timestamp.hpp"
#include "config/constants/constants.hpp"
#include "logging/log_macros.hpp"
#include "ui/utils/error.hpp"

REGISTER_LOG_CATEGORY("UI.MenuBar.Utilities")

namespace ui
{

    /**
     * @brief Construct a new Utilities Menu:: Utilities Menu object
     *
     * @param menuBar
     * @param captureRoot The main window to capture for screenshots
     * @param screenshotShortcut The keyboard shortcut for taking screenshots
     */
    UtilitiesMenu::UtilitiesMenu(
        QMenuBar&          menuBar,
        QMainWindow*       captureRoot,
        const ShortcutSet& screenshotShortcut
    )
        : QObject{&menuBar}, _captureRoot{captureRoot}
    {
        _utilitiesMenu    = menuBar.addMenu("&Utilities");
        _screenshotAction = _utilitiesMenu->addAction("&Screenshot");
        common::setShortcut(_screenshotAction, screenshotShortcut);

        connect(
            _screenshotAction,
            &QAction::triggered,
            this,
            &UtilitiesMenu::requestScreenshot
        );
    }

    /**
     * @brief Requests a screenshot of the current application window and saves
     * it to a user-specified location.
     *
     */
    void UtilitiesMenu::requestScreenshot()
    {
        // 1. Capture immediately — the shortcut press itself didn't move the
        //    mouse, so hover/selection state is still exactly as it was.
        QPixmap pixmap = _captureRoot->grab();

        std::vector<QWidget*> overlays;
        for (QWidget* top : QApplication::topLevelWidgets())
        {
            if (top == _captureRoot)
                continue;
            if (!top->isVisible())
                continue;
            overlays.push_back(top);
        }

        QPainter painter(&pixmap);
        for (QWidget* overlay : overlays)
        {
            const QPixmap overlayPixmap = overlay->grab();
            const QPoint  relativePos   = overlay->mapToGlobal(QPoint(0, 0)) -
                                       _captureRoot->mapToGlobal(QPoint(0, 0));
            painter.drawPixmap(relativePos, overlayPixmap);
        }
        painter.end();

        if (pixmap.isNull())
        {
            const auto msg = std::format(
                "Screen capture returned a null pixmap (possible "
                "Wayland/portal restriction, session type: {})",
                std::string(qgetenv("XDG_SESSION_TYPE"))
            );
            LOG_ERROR(msg);
            ErrorDialog::show("Screenshot Error", QString::fromStdString(msg));
            return;
        }

        // 2. Only now prompt for a save location — the image is already safely
        //    captured and held in memory, so there's no time pressure on it.
        const auto path = Constants::getInstance().getImagesPath().string();
        const auto timestamp = Timestamp().fileSafe();
        const auto filename =
            Constants::getFilePrefix() + "_" + timestamp + ".png";

        const QString filePath = QFileDialog::getSaveFileName(
            nullptr,
            "Save Screenshot",
            QString::fromStdString(path + "/" + filename),
            "PNG Files (*.png)"
        );

        if (filePath.isEmpty())
        {
            LOG_INFO("Screenshot save canceled by user");
            return;
        }

        if (!pixmap.save(filePath, "PNG"))
        {
            const auto msg = std::format(
                "Failed to save screenshot to '{}'",
                filePath.toStdString()
            );
            LOG_ERROR(msg);
            ErrorDialog::show("Screenshot Error", QString::fromStdString(msg));
        }
        else
        {
            const auto msg =
                std::format("Screenshot saved to '{}'", filePath.toStdString());

            LOG_INFO(msg);
        }
    }

}   // namespace ui