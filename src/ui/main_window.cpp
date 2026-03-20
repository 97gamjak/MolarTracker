#include "main_window.hpp"

#include <QApplication>
#include <QDialog>
#include <QLabel>
#include <QMessageBox>
#include <QStackedWidget>
#include <QStatusBar>
#include <QTabWidget>
#include <QVBoxLayout>

#include "app/app_context.hpp"
#include "config/constants.hpp"
#include "ui/controller/ensure_profile_controller.hpp"
#include "ui/handlers/handlers.hpp"
#include "ui/widgets/menu_bar/menu_bar.hpp"
#include "ui/widgets/side_bar/account_category.hpp"
#include "ui/widgets/side_bar/side_bar.hpp"

namespace ui
{

    /**
     * @brief Construct a new Main Window:: Main Window object
     *
     * @param appContext The application context
     * @param handlers The handlers
     */
    MainWindow::MainWindow(app::AppContext& appContext, Handlers& handlers)
        : _appContext{appContext}, _handlers{handlers}
    {
        _handlers.getDirtyStateHandler().subscribe(appContext, this);

        setWindowTitle(false);

        const auto size = Constants::getMainWindowSize();
        resize(size.first, size.second);

        _buildUI();
    }

    /**
     * @brief Start the main window (show it)
     *
     */
    void MainWindow::start() { _ensureProfileExists(); }

    /**
     * @brief Build the UI of the main window
     *
     */
    void MainWindow::_buildUI()
    {
        _buildMenuBar();

        // NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
        auto* root = new QWidget{this};
        setCentralWidget(root);

        // NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
        auto* layout = new QHBoxLayout{root};

        const auto& margins = Constants::getCoreWindowMargins();
        layout->setContentsMargins(
            std::get<0>(margins),
            std::get<1>(margins),
            std::get<2>(margins),
            std::get<3>(margins)
        );
        layout->setSpacing(0);

        // NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
        _sideBar = new SideBar{this};
        _sideBar->setFixedWidth(Constants::getSideBarWidth());

        // NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
        _centralStack = new QStackedWidget{this};

        layout->addWidget(_sideBar);
        layout->addWidget(_centralStack);

        _sideBarController =
            std::make_unique<SideBarController>(this, _sideBar, _centralStack);
    }

    /**
     * @brief Build the menu bar and its controllers
     *
     */
    void MainWindow::_buildMenuBar()
    {
        // NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
        _menuBar = new MenuBar{this};
        setMenuBar(_menuBar);

        _menuBarController = std::make_unique<MenuBarController>(
            this,
            *_menuBar,
            _appContext,
            _undoStack
        );
    }

    /**
     * @brief Ensure that a profile exists, and if not, prompt the user to
     * create one
     *
     */
    void MainWindow::_ensureProfileExists()
    {
        _ensureProfileController = std::make_unique<EnsureProfileController>(
            *this,
            _appContext,
            _undoStack
        );

        _ensureProfileController->ensureProfileExists();
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

}   // namespace ui
