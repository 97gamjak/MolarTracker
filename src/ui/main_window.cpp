#include "main_window.hpp"

#include <QApplication>
#include <QLabel>
#include <QMenuBar>
#include <QStatusBar>
#include <QTabWidget>
#include <QToolBar>
#include <QVBoxLayout>
#include <QWidget>

#include "top_menu_bar.hpp"

MainWindow::MainWindow(QWidget* parent) : QMainWindow{parent}
{
    setWindowTitle("App");
    resize(1100, 700);

    _topMenuBar = new TopMenuBar{*this};
    _topMenuBar->build();

    buildCentral();

    statusBar()->showMessage("Ready");
}

void MainWindow::buildCentral()
{
    auto* root = new QWidget{this};
    setCentralWidget(root);

    auto* layout = new QVBoxLayout{root};
    layout->setContentsMargins(8, 8, 8, 8);

    auto* tabs = new QTabWidget{root};
    layout->addWidget(tabs);

    // Placeholder pages
    tabs->addTab(new QLabel{"Home (placeholder)"}, "Home");
    tabs->addTab(new QLabel{"Data (placeholder)"}, "Data");
    tabs->addTab(new QLabel{"Tools (placeholder)"}, "Tools");
}
