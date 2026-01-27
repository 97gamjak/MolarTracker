#include "file_menu.hpp"

#include <QMainWindow>
#include <QMenuBar>
#include <QObject>

namespace ui
{

    FileMenu::FileMenu(QMainWindow& mainWindow)
        : QObject{&mainWindow}, _mainWindow{mainWindow}
    {
    }

    void FileMenu::build(QMenu* parentMenu)
    {
        _saveAction = parentMenu->addAction("&Save");
        _saveAction->setShortcut(QKeySequence::Save);
        connect(_saveAction, &QAction::triggered, this, &FileMenu::requestSave);

        _quitAction = parentMenu->addAction("&Quit");
        _quitAction->setShortcut(QKeySequence::Quit);
        connect(_quitAction, &QAction::triggered, this, &FileMenu::requestQuit);
    }

}   // namespace ui