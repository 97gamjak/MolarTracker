#ifndef __UI__INCLUDE__UI__MAIN_WINDOW_HPP__
#define __UI__INCLUDE__UI__MAIN_WINDOW_HPP__

#include <QMainWindow>

namespace app
{
    class AppContext;   // Forward declaration
}   // namespace app

class QStackedWidget;   // Forward declaration

namespace ui
{
    class MenuBar;   // Forward declaration
    class SideBar;   // Forward declaration

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
        MenuBar* _menuBar = nullptr;
        /// Pointer to side bar widget
        SideBar* _sideBar = nullptr;
        /// Pointer to the central stacked widget
        QStackedWidget* _centralStack = nullptr;

        /// Undo stack for managing undoable commands in the application

       public:
        explicit MainWindow();

        void setWindowTitle(const bool& isDirty);

        [[nodiscard]] MenuBar&        getMenuBar();
        [[nodiscard]] SideBar&        getSideBar();
        [[nodiscard]] QStackedWidget* getCentralWidget();

       private:
        void _buildUI();
        void _buildMenuBar();
    };

}   // namespace ui

#endif   // __UI__INCLUDE__UI__MAIN_WINDOW_HPP__