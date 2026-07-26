#include "ui/menu_bar/file_menu.hpp"

#include <QAction>
#include <QKeySequence>
#include <QMenu>
#include <QMenuBar>
#include <QObject>

#include "common/shortcut.hpp"

namespace ui
{

    /**
     * @brief Construct a new File Menu:: File Menu object
     *
     * @param menuBar
     * @param saveShortcut
     * @param quitShortcut
     */
    FileMenu::FileMenu(
        QMenuBar&       menuBar,
        const Shortcut& saveShortcut,
        const Shortcut& quitShortcut
    )
        : QObject{&menuBar}
    {
        _fileMenu = menuBar.addMenu("&File");

        _saveAction = _fileMenu->addAction("&Save");
        _saveAction->setShortcut(saveShortcut.toQKeySequence());
        connect(_saveAction, &QAction::triggered, this, &FileMenu::requestSave);

        _quitAction = _fileMenu->addAction("&Quit");
        _quitAction->setShortcut(quitShortcut.toQKeySequence());
        connect(_quitAction, &QAction::triggered, this, &FileMenu::requestQuit);
    }

}   // namespace ui