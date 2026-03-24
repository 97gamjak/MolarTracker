#include "main_window.hpp"

#include <QApplication>
#include <QDialog>
#include <QLabel>
#include <QMessageBox>
#include <QStackedWidget>
#include <QStatusBar>
#include <QTabWidget>
#include <QVBoxLayout>

#include "config/constants.hpp"
#include "ui/widgets/menu_bar/menu_bar.hpp"
#include "ui/widgets/side_bar/account_category.hpp"
#include "ui/widgets/side_bar/side_bar.hpp"
#include "utils/qt_helpers.hpp"

namespace ui
{

    /**
     * @brief Construct a new Main Window:: Main Window object
     */
    MainWindow::MainWindow()
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
        _buildMenuBar();

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

        _sideBar = utils::makeQChild<SideBar>(this);
        _sideBar->setFixedWidth(Constants::getSideBarWidth());

        _centralStack = utils::makeQChild<QStackedWidget>(this);

        layout->addWidget(_sideBar);
        layout->addWidget(_centralStack);
    }

    /**
     * @brief Build the menu bar and its controllers
     *
     */
    void MainWindow::_buildMenuBar()
    {
        _menuBar = utils::makeQChild<MenuBar>(this);
        setMenuBar(_menuBar);
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

    MenuBar& MainWindow::getMenuBar() { return *_menuBar; }

    SideBar& MainWindow::getSideBar() { return *_sideBar; }

    QStackedWidget* MainWindow::getCentralWidget() { return _centralStack; }

}   // namespace ui
