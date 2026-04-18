#include "ui/main_window.hpp"

#include <QApplication>
#include <QLabel>
#include <QMessageBox>
#include <QStackedWidget>
#include <QStatusBar>
#include <QTabWidget>
#include <QVBoxLayout>

#include "config/constants.hpp"
#include "ui/menu_bar/menu_bar.hpp"
#include "ui/side_bar/account_category.hpp"
#include "ui/side_bar/side_bar.hpp"
#include "utils/qt_helpers.hpp"

namespace ui
{

    /**
     * @brief Construct a new Main Window:: Main Window object
     */
    MainWindow::MainWindow()
        : _menuBar(new MenuBar(this)),
          _sideBar(new SideBar(this)),
          _centralWidget(new CentralWidget(this))
    {
        setWindowTitle(false);

        const auto size = Constants::getMainWindowSize();
        resize(size.first, size.second);

        _buildUI();
    }

    /**
     * @brief Build the UI of the main window
     *
     */
    void MainWindow::_buildUI()
    {
        setMenuBar(_menuBar);

        auto* root = utils::makeQChild<QWidget>(this);
        setCentralWidget(root);

        auto* layout = utils::makeQChild<QHBoxLayout>(root);

        const auto& margins = Constants::getCoreWindowMargins();
        layout->setContentsMargins(
            std::get<0>(margins),
            std::get<1>(margins),
            std::get<2>(margins),
            std::get<3>(margins)
        );
        layout->setSpacing(0);

        _sideBar->setFixedWidth(Constants::getSideBarWidth());

        layout->addWidget(_sideBar);
        layout->addWidget(_centralWidget);
    }

    /**
     * @brief Set the window title of the main window, this will append a
     * warning to the title if there are unsaved changes in the application
     *
     * @param isDirty Whether there are unsaved changes
     */
    void MainWindow::setWindowTitle(const bool& isDirty)
    {
        QApplication::setApplicationDisplayName("");
        auto baseTitle = Constants::getAppDisplayName();

        if (isDirty)
            baseTitle += " *** unsaved changes ***";

        const auto name = QString::fromStdString(baseTitle);
        QMainWindow::setWindowTitle(name);
    }

    /**
     * @brief getter for the menu bar
     *
     * @return MenuBar&
     */
    MenuBar& MainWindow::getMenuBar() { return *_menuBar; }

    /**
     * @brief getter for the side bar
     *
     * @return SideBar&
     */
    SideBar& MainWindow::getSideBar() { return *_sideBar; }

    /**
     * @brief getter for the central widget
     *
     * @return CentralWidget*
     */
    CentralWidget* MainWindow::getCentralWidget() { return _centralWidget; }

}   // namespace ui
