#include "top_menu_bar.hpp"

#include <QMainWindow>
#include <QMenuBar>
#include <QObject>

namespace ui
{

    TopMenuBar::TopMenuBar(QMainWindow& mainWindow)
        : QObject{&mainWindow}, _mainWindow{mainWindow}
    {
    }

    void TopMenuBar::build()
    {
        auto* bar = _mainWindow.menuBar();

        _buildFileMenu(bar);
        _buildEditMenu(bar);
        _buildSettingsMenu(bar);
        _buildDebugMenu(bar);
        _buildHelpMenu(bar);
    }

    void TopMenuBar::_buildFileMenu(QMenuBar* menu)
    {
        auto* fileMenu = menu->addMenu("&File");

        _saveAction = fileMenu->addAction("&Save");
        _saveAction->setShortcut(QKeySequence::Save);
        connect(
            _saveAction,
            &QAction::triggered,
            this,
            &TopMenuBar::requestSave
        );

        _quitAction = fileMenu->addAction("&Quit");
        _quitAction->setShortcut(QKeySequence::Quit);
        connect(
            _quitAction,
            &QAction::triggered,
            this,
            &TopMenuBar::requestQuit
        );
    }

    void TopMenuBar::_buildEditMenu(QMenuBar* menu)
    {
        auto* editMenu = menu->addMenu("&Edit");

        _undoAction = editMenu->addAction("&Undo");
        _undoAction->setShortcut(QKeySequence::Undo);
        _undoAction->setEnabled(false);
        connect(
            _undoAction,
            &QAction::triggered,
            this,
            &TopMenuBar::requestUndo
        );

        _redoAction = editMenu->addAction("&Redo");
        _redoAction->setShortcut(QKeySequence::Redo);
        _redoAction->setEnabled(false);
        connect(
            _redoAction,
            &QAction::triggered,
            this,
            &TopMenuBar::requestRedo
        );
    }

    void TopMenuBar::_buildSettingsMenu(QMenuBar* menu)
    {
        auto* settingsMenu = menu->addMenu("&Settings");

        _preferencesAction = settingsMenu->addAction("&Preferences");
        connect(
            _preferencesAction,
            &QAction::triggered,
            this,
            &TopMenuBar::requestPreferences
        );
    }

    void TopMenuBar::_buildDebugMenu(QMenuBar* menu)
    {
        auto* debugMenu = menu->addMenu("&Debug");

        _debugAction = debugMenu->addAction("&Debug Flags");
        connect(
            _debugAction,
            &QAction::triggered,
            this,
            &TopMenuBar::requestDebug
        );
    }

    void TopMenuBar::_buildHelpMenu(QMenuBar* menu)
    {
        auto* helpMenu = menu->addMenu("&Help");

        _aboutAction = helpMenu->addAction("&About");
        connect(
            _aboutAction,
            &QAction::triggered,
            this,
            &TopMenuBar::requestAbout
        );
    }

    void TopMenuBar::setUndoEnabled(bool enabled)
    {
        _undoAction->setEnabled(enabled);
    }

    void TopMenuBar::setRedoEnabled(bool enabled)
    {
        _redoAction->setEnabled(enabled);
    }

    void TopMenuBar::setUndoText(const QString& text)
    {
        _undoAction->setText(text);
    }

    void TopMenuBar::setRedoText(const QString& text)
    {
        _redoAction->setText(text);
    }

}   // namespace ui