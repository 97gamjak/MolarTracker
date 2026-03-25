#include "controller/main_controller.hpp"

#include "app/app_context.hpp"
#include "commands/undo_stack.hpp"
#include "config/constants.hpp"
#include "controller/ensure_profile_controller.hpp"
#include "controller/handlers/handlers.hpp"
#include "controller/menu_bar/menu_bar_controller.hpp"
#include "controller/side_bar_controller.hpp"
#include "logging/log_manager.hpp"
#include "settings/settings.hpp"
#include "ui/main_window.hpp"

namespace controller
{
    struct MainController::Impl
    {
        settings::Settings _settings;
        app::AppContext    _appContext;
        ui::MainWindow     _mainWindow;
        cmd::UndoStack     _undoStack;

        controller::Handlers          _handlers;
        controller::MenuBarController _menuBarController;
        controller::SideBarController _sideBarController;

        Impl()
            : _settings(Constants::getInstance().getConfigPath()),
              _appContext(_settings),
              _handlers(_settings),
              _menuBarController(
                  &_mainWindow,
                  _mainWindow.getMenuBar(),
                  _appContext,
                  _undoStack
              ),
              _sideBarController(
                  _undoStack,
                  _appContext,
                  &_mainWindow,
                  &_mainWindow.getSideBar(),
                  _mainWindow.getCentralWidget()
              )
        {
            _handlers.getDirtyStateHandler().subscribe(
                _appContext,
                &_mainWindow
            );
        }
    };

    MainController::MainController() : _impl{std::make_unique<Impl>()}
    {
        // initialize settings
        auto& loggingSettings = _impl->_settings.getLoggingSettings();

        // initialize ring file buffered logger
        logging::LogManager::getInstance().initializeCategories();
        logging::LogManager::getInstance().initializeRingFileLogger(
            loggingSettings,
            Constants::getInstance().getDataPath()
        );
    }

    MainController::~MainController() = default;

    void MainController::start()
    {
        _impl->_mainWindow.show();

        auto controller = controller::EnsureProfileController{
            _impl->_mainWindow,
            _impl->_appContext,
            _impl->_undoStack
        };

        controller.ensureProfileExists();
    }

}   // namespace controller