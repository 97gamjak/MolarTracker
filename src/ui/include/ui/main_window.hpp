#ifndef __UI__INCLUDE__UI__MAIN_WINDOW_HPP__
#define __UI__INCLUDE__UI__MAIN_WINDOW_HPP__

#include <QMainWindow>

#include "ui/central/central_widget.hpp"

namespace app
{
    class AppContext;   // Forward declaration
}   // namespace app

class QStackedWidget;   // Forward declaration

namespace ui
{
    class MenuBar;         // Forward declaration
    class SideBar;         // Forward declaration
    class CentralWidget;   // Forward declaration

    /**
     * @brief The main window of the application. This is the central widget
     * that contains the menu bar and the central widget.
     *
     */
    class MainWindow final : public QMainWindow
    {
        Q_OBJECT

       private:
        /// Pointer to the menu bar widget
        MenuBar* _menuBar;
        /// Pointer to side bar widget
        SideBar* _sideBar;
        /// Pointer to the central widget
        CentralWidget* _centralWidget;

        /// Undo stack for managing undoable commands in the application

       public:
        explicit MainWindow();

        void setWindowTitle(const bool& isDirty);

        [[nodiscard]] MenuBar&       getMenuBar();
        [[nodiscard]] SideBar&       getSideBar();
        [[nodiscard]] CentralWidget* getCentralWidget();

       private:
        void _buildUI();
    };

}   // namespace ui

#endif   // __UI__INCLUDE__UI__MAIN_WINDOW_HPP__