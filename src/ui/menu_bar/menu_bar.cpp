#include "menu_bar.hpp"

#include <QMainWindow>
#include <QMenuBar>
#include <QObject>

#include "debug_menu.hpp"
#include "file_menu.hpp"

namespace ui
{

    MenuBar::MenuBar(QWidget* parent)
        : QMenuBar{parent},
          _fileMenu{*this},
          _editMenu{*this},
          _debugMenu{*this},
          _settingsMenu{*this},
          _helpMenu{*this}
    {
    }

}   // namespace ui