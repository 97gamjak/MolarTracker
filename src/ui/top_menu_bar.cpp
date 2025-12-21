#include "top_menu_bar.hpp"

#include <QMainWindow>
#include <QMenuBar>
#include <QObject>

TopMenuBar::TopMenuBar(QMainWindow& mainWindow)
    : _mainWindow{mainWindow}, QObject{&mainWindow}
{
}

void TopMenuBar::build()
{
    auto* bar = _mainWindow.menuBar();

    auto* fileMenu     = bar->addMenu("&File");
    auto* settingsMenu = bar->addMenu("&Settings");
    auto* helpMenu     = bar->addMenu("&Help");
}