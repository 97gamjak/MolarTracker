#include "main_window.hpp"

#include <QLabel>
#include <QMessageBox>
#include <QStatusBar>
#include <QVBoxLayout>

#include "app/app_context.hpp"
#include "domain/profile.hpp"
#include "ui/top_menu_bar.hpp"
#include "ui/undo_redo/toggle_flag_command.hpp"

namespace ui
{

    MainWindow::MainWindow(app::AppContext& appContext, QWidget* parent)
        : QMainWindow{parent}, _appContext{appContext}
    {
        setWindowTitle("Molar Tracker");
        resize(1100, 700);

        _buildUI();

        _undoStack.pushAndRedo(
            std::make_unique<ToggleFlagCommand>(_dummyFlag, "Toggle flag")
        );
        _refreshUndoRedoActions();

        _ensureProfileExists();
    }

    void MainWindow::_buildUI()
    {
        _topMenuBar = new TopMenuBar{*this};
        _topMenuBar->build();

        connect(_topMenuBar, &TopMenuBar::requestQuit, this, &QWidget::close);
        connect(
            _topMenuBar,
            &TopMenuBar::requestUndo,
            this,
            [this]()
            {
                _undoStack.undo();
                statusBar()->showMessage("Undo requested");
                _refreshUndoRedoActions();
            }
        );
        connect(
            _topMenuBar,
            &TopMenuBar::requestRedo,
            this,
            [this]()
            {
                _undoStack.redo();
                statusBar()->showMessage("Redo requested");
                _refreshUndoRedoActions();
            }
        );
        connect(
            _topMenuBar,
            &TopMenuBar::requestPreferences,
            this,
            [this]()
            {
                QMessageBox::information(
                    this,
                    "Preferences",
                    "Preferences requested"
                );
            }
        );
        connect(
            _topMenuBar,
            &TopMenuBar::requestAbout,
            this,
            [this]()
            {
                QMessageBox::information(
                    this,
                    "About Molar Tracker",
                    "Molar Tracker\nVersion 0.1.0\n\n(c) 2025-now Molar "
                    "Tracker "
                    "Contributors: Jakob Gamper"
                );
            }
        );

        _buildCentral();
        _refreshUndoRedoActions();
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

    void MainWindow::_refreshUndoRedoActions()
    {
        // Placeholder implementation
        // In a real application, this would check the undo/redo stack status
        bool canUndo = _undoStack.canUndo();
        bool canRedo = _undoStack.canRedo();

        _topMenuBar->setUndoEnabled(canUndo);
        _topMenuBar->setUndoText(
            canUndo ? QString::fromStdString("Undo " + _undoStack.undoLabel())
                    : "Undo"
        );

        _topMenuBar->setRedoEnabled(canRedo);
        _topMenuBar->setRedoText(
            canRedo ? QString::fromStdString("Redo " + _undoStack.redoLabel())
                    : "Redo"
        );
    }

    void MainWindow::_ensureProfileExists()
    {
        // auto&      profileService = _appContext.services().profileService();
        // const auto config         = _appContext.config();

        // const auto hasDefaultProfile = config.has_default_profile();

        // if (hasDefaultProfile)
        // {
        //     const auto name = config().get_default_profile_name().value();
        //     if (!profileService.profileExists(name))
        //     {
        //         // todo: create error message!!!
        //     }
        //     else
        //     {
        //         statusBar()->showMessage(
        //             QString::fromStdString("Loaded default profile: " + name)
        //         );
        //         return;
        //     }
        // }

        // if (profiles.empty())
        // {
        //     const std::string defaultProfileName = "Default Profile";
        //     profileService.createProfile(defaultProfileName);
        //     statusBar()->showMessage(
        //         QString::fromStdString(
        //             "No profiles found. Created default profile: " +
        //             defaultProfileName
        //         )
        //     );
        //     return;
        // }

        // statusBar()->showMessage(
        //     QString::fromStdString(
        //         "Loaded " + std::to_string(profiles.size()) + " profile(s)"
        //     )
        // );
    }

}   // namespace ui
