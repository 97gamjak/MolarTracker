#include <QApplication>

#include "app/app_context.hpp"
#include "ui/main_window.hpp"

int main(int argc, char** argv)
{
    QApplication app{argc, argv};

    app::AppContext appContext{"molartracker.db"};
    ui::MainWindow  mainWindow{appContext};

    mainWindow.show();

    return app.exec();
}