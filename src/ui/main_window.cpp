#include "main_window.hpp"

#include <QLabel>
#include <QMessageBox>
#include <QStatusBar>
#include <QVBoxLayout>

#include "top_menu_bar.hpp"

MainWindow::MainWindow(QWidget* parent) : QMainWindow{parent}
{
    setWindowTitle("Molar Tracker");
    resize(1100, 700);

    _buildUI();
    statusBar()->showMessage("Ready");
}

void MainWindow::_buildUI()
{
    _topMenuBar = new TopMenuBar{*this};
    _topMenuBar->build();

    connect(_topMenuBar, &TopMenuBar::requestQuit, this, &QWidget::close);
    connect(
        _topMenuBar,
        &TopMenuBar::requestUndo,
        this,
        [this]() { statusBar()->showMessage("Undo requested"); }
    );
    connect(
        _topMenuBar,
        &TopMenuBar::requestRedo,
        this,
        [this]() { statusBar()->showMessage("Redo requested"); }
    );
    connect(
        _topMenuBar,
        &TopMenuBar::requestPreferences,
        this,
        [this]()
        {
            QMessageBox::information(
                this,
                "Preferences",
                "Preferences requested"
            );
        }
    );
    connect(
        _topMenuBar,
        &TopMenuBar::requestAbout,
        this,
        [this]()
        {
            QMessageBox::information(
                this,
                "About Molar Tracker",
                "Molar Tracker\nVersion 0.1.0\n\n(c) 2025-now Molar Tracker "
                "Contributors: Jakob Gamper"
            );
        }
    );

    _buildCentral();
    _refreshUndoRedoActions();
}

void MainWindow::_buildCentral()
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

void MainWindow::_refreshUndoRedoActions()
{
    // Placeholder implementation
    // In a real application, this would check the undo/redo stack status
    bool canUndo = false;
    bool canRedo = false;

    _topMenuBar->setUndoEnabled(canUndo);
    _topMenuBar->setRedoEnabled(canRedo);
}
