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
#include "ui/binders/debug_menu_binder.hpp"
#include "ui/binders/undo_redo_binder.hpp"
#include "ui/menu_bar/menu_bar.hpp"
#include "ui/profile/add_profile_dlg.hpp"
#include "ui/profile/profile_selection_dlg.hpp"
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
        _menuBar = new MenuBar{*this};
        _menuBar->build();

        // clang-format off
        _undoRedoBinder  = new UndoRedoBinder{*this, *_menuBar, _undoStack};
        _debugMenuBinder = new DebugMenuBinder{*this, *_menuBar};
        // clang-format on

        // clang-format off
        connect(_menuBar, &MenuBar::requestQuit, this, &QWidget::close);
        connect(_menuBar, &MenuBar::requestSave, this, &MainWindow::_onSaveRequested);
        connect(_menuBar, &MenuBar::requestPreferences, this, &MainWindow::_onPreferencesRequested);
        connect(_menuBar, &MenuBar::requestAbout, this, &MainWindow::_onAboutRequested);
        // clang-format on
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

    void MainWindow::_onSaveRequested()
    {
        _appContext.getStore().commit();
        _appContext.getSettings().save();
        statusBar()->showMessage("Save requested");
    }

    void MainWindow::_onPreferencesRequested()
    {
        QMessageBox::information(this, "Preferences", "Preferences requested");
    }

    void MainWindow::_onAboutRequested()
    {
        QMessageBox::information(
            this,
            "About Molar Tracker",
            "Molar Tracker\nVersion 0.1.0\n\n(c) 2025-now Molar Tracker "
            "Contributors: Jakob Gamper"
        );
    }

    // TODO: rename to _loadInitialProfile or similar
    void MainWindow::_ensureProfileExists()
    {
        auto& profileStore = _appContext.getStore().getProfileStore();
        auto& settings     = _appContext.getSettings();

        if (settings.has_default_profile())
        {
            const auto name = settings.get_default_profile_name().value();
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
                auto* dialog = new AddProfileDialog{
                    profileStore,
                    settings,
                    _undoStack,
                    this
                };
                dialog->setWindowTitle("Create your first profile");
                dialog->setModal(true);
                dialog->setAttribute(Qt::WA_DeleteOnClose);
                dialog->setEnforceDefaultProfile(true);
                utils::moveDialogToParentScreenCenter(dialog, this);

                connect(
                    dialog,
                    &QDialog::accepted,
                    this,
                    [this, dialog]()
                    {
                        const auto profile   = dialog->getProfile();
                        auto&      _settings = _appContext.getSettings();
                        _settings.set_default_profile_name(profile.name);
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
