#include "main_window.hpp"

#include <QApplication>
#include <QDialog>
#include <QLabel>
#include <QMessageBox>
#include <QStatusBar>
#include <QTabWidget>
#include <QVBoxLayout>

#include "app/app_context.hpp"
#include "config/constants.hpp"
#include "ui/controller/ensure_profile_controller.hpp"
#include "ui/handlers/handlers.hpp"
#include "ui/widgets/menu_bar/menu_bar.hpp"
#include "ui/widgets/side_bar/account_category.hpp"
#include "ui/widgets/side_bar/overview_category.hpp"
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

        resize(
            Constants::getMainWindowSize().first,
            Constants::getMainWindowSize().second
        );

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
        auto* layout = new QVBoxLayout{root};

        const auto& margins = Constants::getCoreWindowMargins();
        layout->setContentsMargins(
            std::get<0>(margins),
            std::get<1>(margins),
            std::get<2>(margins),
            std::get<3>(margins)
        );

        _sideBar       = new SideBar{this};
        auto* overview = new OverviewCategory{this};
        auto* accounts = new AccountCategory{this};

        _sideBar->registerCategory(overview);
        _sideBar->registerCategory(accounts);

        accounts->refresh();

        // connect(
        //     accounts,
        //     &SideBar::itemSelected,
        //     this,
        //     [/*this*/](int /*id*/)
        //     {
        //         // TODO(97gamjak): swap main content area
        //     }
        // );

        layout->addWidget(_sideBar);
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

        _fileMenuController = std::make_unique<FileMenuController>(
            *this,
            _menuBar->getFileMenu(),
            _appContext
        );

        _editMenuController = std::make_unique<EditMenuController>(
            *this,
            _menuBar->getEditMenu(),
            _undoStack
        );

        _debugMenuController = std::make_unique<DebugMenuController>(
            *this,
            _menuBar->getDebugMenu(),
            _appContext,
            _undoStack
        );

        _settingsMenuController = std::make_unique<SettingsMenuController>(
            *this,
            _menuBar->getSettingsMenu()
        );

        _helpMenuController = std::make_unique<HelpMenuController>(
            *this,
            _menuBar->getHelpMenu()
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
