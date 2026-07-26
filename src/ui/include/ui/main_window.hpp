#ifndef __UI__INCLUDE__UI__MAIN_WINDOW_HPP__
#define __UI__INCLUDE__UI__MAIN_WINDOW_HPP__

#include <QMainWindow>
#include <functional>

#include "ui/central/central_widget.hpp"

class QCloseEvent;   // Forward declaration

namespace settings
{
    class ShortcutSettings;   // Forward declaration
}   // namespace settings

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

       public:
        /// Callback type used to ask whether the window is allowed to close
        using CanCloseCallback = std::function<bool()>;

       private:
        /// Pointer to the menu bar widget
        MenuBar* _menuBar;
        /// Pointer to side bar widget
        SideBar* _sideBar;
        /// Pointer to the central widget
        CentralWidget* _centralWidget;
        /// Optional callback invoked in closeEvent to guard against closing
        /// with unsaved changes
        CanCloseCallback _canCloseCallback;

       public:
        explicit MainWindow(const settings::ShortcutSettings& shortcutSettings);

        void setWindowTitle(const bool& isDirty);
        void setCanCloseCallback(CanCloseCallback callback);

        [[nodiscard]] MenuBar&       getMenuBar();
        [[nodiscard]] SideBar&       getSideBar();
        [[nodiscard]] CentralWidget* getCentralWidget();

       protected:
        void closeEvent(QCloseEvent* event) override;

       private:
        void _buildUI();
    };

}   // namespace ui

#endif   // __UI__INCLUDE__UI__MAIN_WINDOW_HPP__