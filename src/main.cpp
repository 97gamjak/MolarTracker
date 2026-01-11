#include <QApplication>

#include "app/app_context.hpp"
#include "ui/main_window.hpp"

int main(int argc, char** argv)
{
    QApplication app{argc, argv};

    app.setApplicationName("Molar Tracker");
    app.setApplicationDisplayName("Molar Tracker");
    app.setOrganizationName("Molar Tracker Contributors");

    app.setDesktopFileName(MOLARTRACKER_DESKTOP_APP_NAME);

    app::AppContext appContext;
    ui::MainWindow  mainWindow{appContext};

    mainWindow.show();

    return app.exec();
}