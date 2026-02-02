#include "file_menu.hpp"

#include <QMainWindow>
#include <QMenuBar>
#include <QObject>

namespace ui
{

    FileMenu::FileMenu(QMenuBar& menuBar) : QObject{&menuBar}
    {
        _fileMenu = menuBar.addMenu("&File");

        _saveAction = _fileMenu->addAction("&Save");
        _saveAction->setShortcut(QKeySequence::Save);
        connect(_saveAction, &QAction::triggered, this, &FileMenu::requestSave);

        _quitAction = _fileMenu->addAction("&Quit");
        _quitAction->setShortcut(QKeySequence::Quit);
        connect(_quitAction, &QAction::triggered, this, &FileMenu::requestQuit);
    }

}   // namespace ui