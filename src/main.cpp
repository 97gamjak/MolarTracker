#include <QApplication>

#include "app_context.hpp"
#include "main_window.hpp"

int main(int argc, char** argv)
{
    QApplication app{argc, argv};

    AppContext appContext{"molartracker.db"};
    MainWindow mainWindow{appContext};

    mainWindow.show();

    return app.exec();
}