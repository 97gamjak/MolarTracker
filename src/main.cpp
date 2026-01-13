#include <QApplication>

#include "app/app_context.hpp"
#include "config/constants.hpp"
#include "ui/main_window.hpp"

int main(int argc, char** argv)
{
    QApplication app{argc, argv};

    app.setApplicationName(Constants::APP_NAME);
    app.setApplicationDisplayName(Constants::APP_NAME);

    app.setDesktopFileName(Constants::DESKTOP_APP_NAME);

    app::AppContext appContext;
    ui::MainWindow  mainWindow{appContext};

    mainWindow.show();

    return app.exec();
}