#ifndef __UI__MAIN_WINDOW_HPP__
#define __UI__MAIN_WINDOW_HPP__

#include <QMainWindow>

#include "undo_redo/undo_stack.hpp"

class TopMenuBar;

class MainWindow final : public QMainWindow
{
    Q_OBJECT

   private:
    TopMenuBar* _topMenuBar = nullptr;

    UndoStack _undoStack;
    bool      _dummyFlag = false;

   private:
    void _buildUI();
    void _buildCentral();

    void _refreshUndoRedoActions();

   public:
    explicit MainWindow(QWidget* parent = nullptr);
};

#endif   // __UI__MAIN_WINDOW_HPP__