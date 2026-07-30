#include "ui/menu_bar/file_menu.hpp"

#include <QAction>
#include <QKeySequence>
#include <QMenu>
#include <QMenuBar>
#include <QObject>

#include "common/qt_helpers.hpp"

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
        QMenuBar&          menuBar,
        const ShortcutSet& saveShortcut,
        const ShortcutSet& quitShortcut
    )
        : QObject{&menuBar}
    {
        _fileMenu = menuBar.addMenu("&File");

        _saveAction = _fileMenu->addAction("&Save");
        common::setShortcut(_saveAction, saveShortcut);
        connect(_saveAction, &QAction::triggered, this, &FileMenu::requestSave);

        _quitAction = _fileMenu->addAction("&Quit");
        common::setShortcut(_quitAction, quitShortcut);
        connect(_quitAction, &QAction::triggered, this, &FileMenu::requestQuit);
    }

}   // namespace ui