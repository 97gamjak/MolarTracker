#include "main_window.hpp"

#include <QDialog>
#include <QLabel>
#include <QMessageBox>
#include <QStatusBar>
#include <QTabWidget>
#include <QVBoxLayout>

#include "app/app_context.hpp"
#include "domain/profile.hpp"
#include "drafts/profile_draft.hpp"
#include "ui/controller/ensure_profile_controller.hpp"
#include "ui/widgets/menu_bar/menu_bar.hpp"
#include "ui/widgets/profile/add_profile_dlg.hpp"
#include "ui/widgets/profile/profile_selection_dlg.hpp"
#include "utils/qt_helpers.hpp"

namespace ui
{

    MainWindow::MainWindow(app::AppContext& appContext, QWidget* parent)
        : QMainWindow{parent}, _appContext{appContext}
    {
        setWindowTitle("Molar Tracker");
        resize(5000, 3000);
        _buildUI();

        _ensureProfileExists();
    }

    void MainWindow::_buildUI()
    {
        _buildMenuBar();
        _buildCentral();
    }

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
