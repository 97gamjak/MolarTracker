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
#include "ui/widgets/profile/add_profile_dlg.hpp"
#include "ui/widgets/profile/profile_selection_dlg.hpp"
#include "utils/qt_helpers.hpp"

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
        resize(5000, 3000);
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
        _buildCentral();
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
     * @brief Build the central widget of the main window
     *
     */
    void MainWindow::_buildCentral()
    {
        // NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
        auto* root = new QWidget{this};
        setCentralWidget(root);

        // NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
        auto* layout = new QVBoxLayout{root};
        layout->setContentsMargins(8, 8, 8, 8);

        // NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
        auto* tabs = new QTabWidget{root};
        layout->addWidget(tabs);

        // Placeholder pages
        // NOLINTBEGIN(cppcoreguidelines-owning-memory)
        tabs->addTab(new QLabel{"Home (placeholder)"}, "Home");
        tabs->addTab(new QLabel{"Data (placeholder)"}, "Data");
        tabs->addTab(new QLabel{"Tools (placeholder)"}, "Tools");
        // NOLINTEND(cppcoreguidelines-owning-memory)
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
