#include "main_window.hpp"

#include <QLabel>
#include <QMessageBox>
#include <QStatusBar>
#include <QVBoxLayout>

#include "app/app_context.hpp"
#include "domain/profile.hpp"
#include "drafts/profile_draft.hpp"
#include "ui/binders/undo_redo_binder.hpp"
#include "ui/profile/add_profile_dlg.hpp"
#include "ui/profile/profile_selection_dlg.hpp"
#include "ui/top_menu_bar.hpp"
#include "ui/undo_redo/toggle_flag_command.hpp"

namespace ui
{

    MainWindow::MainWindow(app::AppContext& appContext, QWidget* parent)
        : QMainWindow{parent}, _appContext{appContext}
    {
        setWindowState(Qt::WindowFullScreen);
        setWindowTitle("Molar Tracker");
        resize(1100, 700);

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
        _topMenuBar = new TopMenuBar{*this};
        _topMenuBar->build();

        _undoRedoBinder = new UndoRedoBinder{*this, *_topMenuBar, _undoStack};

        // clang-format off
        connect(_topMenuBar, &TopMenuBar::requestQuit, this, &QWidget::close);
        connect(_topMenuBar, &TopMenuBar::requestSave, this, &MainWindow::_onSaveRequested);
        connect(_topMenuBar, &TopMenuBar::requestPreferences, this, &MainWindow::_onPreferencesRequested);
        connect(_topMenuBar, &TopMenuBar::requestAbout, this, &MainWindow::_onAboutRequested);
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
