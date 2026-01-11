#include "main_window.hpp"

#include <QLabel>
#include <QMessageBox>
#include <QStatusBar>
#include <QVBoxLayout>

#include "app/app_context.hpp"
#include "domain/profile.hpp"
#include "drafts/profile_draft.hpp"
#include "ui/profile/add_profile_dlg.hpp"
#include "ui/profile/profile_selection_dlg.hpp"
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
        connect(
            _topMenuBar,
            &TopMenuBar::requestSave,
            this,
            [this]()
            {
                _appContext.getStore().commit();
                statusBar()->showMessage("Save requested");
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

    // TODO: rename to _loadInitialProfile or similar
    void MainWindow::_ensureProfileExists()
    {
        auto& profileStore = _appContext.getStore().getProfileStore();
        auto& config       = _appContext.getConfig();

        if (config.has_default_profile())
        {
            const auto name = config.get_default_profile_name().value();
            if (!profileStore.profileExists(name))
            {
                // TODO: create error message!!!
            }
            else
            {
                const auto result = profileStore.setActiveProfile(name);
                if (result != app::ProfileStoreResult::Ok)
                {
                    // TODO: create error message!!!
                    return;
                }
                else
                {
                    statusBar()->showMessage(
                        QString::fromStdString(
                            "Loaded default profile: " + name
                        )
                    );
                    return;
                }
            }
        }
        else
        {
            statusBar()->showMessage("No default profile configured.");
            if (profileStore.hasProfiles())
            {
                const auto profileNames = profileStore.getAllProfileNames();
                ProfileSelectionDialog dialog{this, profileNames};
                if (dialog.exec() == QDialog::Accepted)
                {
                    // do stuff
                    statusBar()->showMessage("Profile selected.");
                }
                else
                {
                    statusBar()->showMessage("Profile selection canceled.");
                }
            }
            else
            {
                statusBar()->showMessage("No profiles found.");
                auto* dialog = new AddProfileDialog{profileStore, this};
                dialog->setAsActive(true);
                dialog->setWindowTitle("Create your first profile");
                qDebug() << "qt windowtitle" << dialog->windowTitle();
                dialog->setAttribute(Qt::WA_DeleteOnClose);

                connect(
                    dialog,
                    &QDialog::accepted,
                    this,
                    [this, dialog]()
                    {
                        const auto profile = dialog->getProfile();
                        auto&      _config = _appContext.getConfig();
                        _config.set_default_profile_name(profile.name);
                        statusBar()->showMessage("Profile added.");
                    }
                );

                dialog->show();
                dialog->raise();
                dialog->activateWindow();
            }
        }
    }

}   // namespace ui
