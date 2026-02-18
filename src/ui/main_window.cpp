#include "main_window.hpp"

#include <QDialog>
#include <QLabel>
#include <QMessageBox>
#include <QStatusBar>
#include <QTabWidget>
#include <QVBoxLayout>

#include "app/app_context.hpp"
#include "ui/controller/ensure_profile_controller.hpp"
#include "ui/widgets/menu_bar/menu_bar.hpp"
#include "ui/widgets/profile/add_profile_dlg.hpp"
#include "ui/widgets/profile/profile_selection_dlg.hpp"
#include "utils/qt_helpers.hpp"

namespace ui
{

    /**
     * @brief Construct a new Main Window:: Main Window object
     *
     * @param appContext
     * @param controllers
     * @param parent
     */
    MainWindow::MainWindow(
        app::AppContext& appContext,
        Controllers&     controllers,
        QWidget*         parent
    )
        : QMainWindow{parent},
          _appContext{appContext},
          _controllers{controllers}
    {
        setWindowTitle("Molar Tracker");
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
        auto* root = new QWidget{this};
        setCentralWidget(root);

        auto* layout = new QVBoxLayout{root};
        layout->setContentsMargins(8, 8, 8, 8);

        auto* tabs = new QTabWidget{root};
        layout->addWidget(tabs);

        // Placeholder pages
        tabs->addTab(new QLabel{"Home (placeholder)"}, "Home");
        tabs->addTab(new QLabel{"Data (placeholder)"}, "Data");
        tabs->addTab(new QLabel{"Tools (placeholder)"}, "Tools");
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

}   // namespace ui
