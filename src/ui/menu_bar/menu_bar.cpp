#include "menu_bar.hpp"

#include <QMainWindow>
#include <QMenuBar>
#include <QObject>

#include "debug_menu.hpp"
#include "file_menu.hpp"

namespace ui
{

    MenuBar::MenuBar(QWidget* parent) : QMenuBar{parent}, _fileMenu{*this}
    {
        // _debugMenu = new DebugMenu{*parent};

        // clang-format off
        // connect(_debugMenu, &DebugMenu::requestDebugSlots, this, &MenuBar::requestDebugSlots);
        // connect(_debugMenu, &DebugMenu::requestLogViewer, this, &MenuBar::requestLogViewer);
        // clang-format on

        build();
    }

    void MenuBar::build()
    {
        _buildEditMenu(this);
        _buildSettingsMenu(this);
        _buildDebugMenu(this);
        _buildHelpMenu(this);
    }

    void MenuBar::_buildEditMenu(QMenuBar* menu)
    {
        auto* editMenu = menu->addMenu("&Edit");

        _undoAction = editMenu->addAction("&Undo");
        _undoAction->setShortcut(QKeySequence::Undo);
        _undoAction->setEnabled(false);
        connect(_undoAction, &QAction::triggered, this, &MenuBar::requestUndo);

        _redoAction = editMenu->addAction("&Redo");
        _redoAction->setShortcut(QKeySequence::Redo);
        _redoAction->setEnabled(false);
        connect(_redoAction, &QAction::triggered, this, &MenuBar::requestRedo);
    }

    void MenuBar::_buildSettingsMenu(QMenuBar* menu)
    {
        auto* settingsMenu = menu->addMenu("&Settings");

        _preferencesAction = settingsMenu->addAction("&Preferences");
        connect(
            _preferencesAction,
            &QAction::triggered,
            this,
            &MenuBar::requestPreferences
        );
    }

    void MenuBar::_buildDebugMenu(QMenuBar* /*menu*/)
    {
        // auto* debugMenu = menu->addMenu("&Debug");

        // _debugMenu->build(debugMenu);
    }

    void MenuBar::_buildHelpMenu(QMenuBar* menu)
    {
        auto* helpMenu = menu->addMenu("&Help");

        _aboutAction = helpMenu->addAction("&About");
        connect(
            _aboutAction,
            &QAction::triggered,
            this,
            &MenuBar::requestAbout
        );
    }

    void MenuBar::setUndoEnabled(bool enabled)
    {
        _undoAction->setEnabled(enabled);
    }

    void MenuBar::setRedoEnabled(bool enabled)
    {
        _redoAction->setEnabled(enabled);
    }

    void MenuBar::setUndoText(const QString& text)
    {
        _undoAction->setText(text);
    }

    void MenuBar::setRedoText(const QString& text)
    {
        _redoAction->setText(text);
    }

}   // namespace ui