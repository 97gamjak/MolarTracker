#include "controller/main_controller.hpp"

#include "commands/undo_stack.hpp"
#include "config/constants/constants.hpp"
#include "controller/account_controller.hpp"
#include "controller/central_controller.hpp"
#include "controller/ensure_profile_controller.hpp"
#include "controller/handlers/handlers.hpp"
#include "controller/menu_bar/menu_bar_controller.hpp"
#include "controller/side_bar/side_bar_controller.hpp"
#include "controller/transaction_controller.hpp"
#include "logging/log_manager.hpp"
#include "settings/settings.hpp"
#include "store/store_container.hpp"
#include "ui/main_window.hpp"
#include "ui/update/update_available_dialog.hpp"
#include "utils/qt_helpers.hpp"
#include "vcs/update_check_service.hpp"

namespace controller
{
    /**
     * @brief Implementation of the MainController class, this class is
     * responsible for initializing and managing the different components of the
     * application, such as the settings, the profile store, the command stack,
     * and the main window. It also serves as a central point for coordinating
     * actions between these components, for example when a command is executed,
     * it can notify the main controller to update the UI or save the settings.
     *
     */
    struct MainController::Impl
    {
        /// settings component
        settings::Settings _settings;
        /// application context
        store::StoreContainer _storeContainer;
        /// main window of the application
        ui::MainWindow _mainWindow;
        /// undo stack for managing commands
        cmd::UndoStack _undoStack;

        /// controller for managing the account
        CentralController _centralController;

        /// handlers for managing interactions no QT signals
        Handlers _handlers;
        /// controller for managing accounts
        AccountController _accountController;
        /// controller for managing transactions
        TransactionController _transactionController;

        /// controller for managing the menu bar
        MenuBarController _menuBarController;
        /// controller for managing the side bar
        SideBarController _sideBarController;

        /// service that checks GitHub for newer releases
        vcs::UpdateCheckService _updateCheckService;

        /**
         * @brief Construct a new Impl object
         *
         * @param settings
         */
        explicit Impl(settings::Settings&& settings)
            : _settings(std::move(settings)),
              _centralController(_mainWindow.getCentralWidget()),
              _handlers(_settings),
              _accountController(
                  _undoStack,
                  _storeContainer.getAccountStore(),
                  _mainWindow.getCentralWidget()
              ),
              _transactionController(
                  _undoStack,
                  _storeContainer.getTransactionStore(),
                  _storeContainer.getAccountStore(),
                  _storeContainer.getStockStore(),
                  _mainWindow.getCentralWidget()
              ),
              _menuBarController(
                  &_mainWindow,
                  _mainWindow.getMenuBar(),
                  _storeContainer,
                  _undoStack,
                  _settings
              ),
              _sideBarController(
                  _undoStack,
                  _storeContainer,
                  &_mainWindow,
                  &_mainWindow.getSideBar(),
                  _mainWindow.getCentralWidget(),
                  _accountController,
                  _transactionController
              )
        {
            _handlers.getDirtyStateHandler()
                .subscribe(_storeContainer, _settings, &_mainWindow);

            QObject::connect(
                &_updateCheckService,
                &vcs::UpdateCheckService::updateAvailable,
                &_mainWindow,
                [this](utils::SemVer latest)
                {
                    const auto& dismissed = _settings.getGeneralSettings()
                                                .getDismissedUpdateVersion()
                                                .getOptional();

                    if (dismissed.has_value() &&
                        *dismissed == latest.toString())
                        return;

                    auto* dialog = utils::makeQChild<ui::UpdateAvailableDialog>(
                        latest,
                        &_mainWindow
                    );
                    dialog->exec();

                    if (dialog->isDismissedForVersion())
                    {
                        _settings.getGeneralSettings()
                            .getDismissedUpdateVersion()
                            .set(latest.toString());
                        _settings.save();
                    }
                }
            );
        }
    };

    /**
     * @brief Construct a new MainController object, this constructor
     * initializes the main controller and its implementation, it also
     * initializes the logging system using the settings from the settings
     * component, and sets up the main window and the handlers for the
     * application.
     *
     */
    MainController::MainController() : _impl{nullptr}
    {
        // NOTE: _impl is initialized with nullptr in order to have logging
        // already initialized while constructing AppContext

        settings::Settings settings{Constants::getInstance().getConfigPath()};
        // initialize settings
        auto& loggingSettings = settings.getLoggingSettings();

        // initialize ring file buffered logger
        logging::LogManager::getInstance().initialize(
            Constants::getInstance().getDataPath().string(),
            loggingSettings
        );

        _impl = std::make_unique<Impl>(std::move(settings));
    }

    /**
     * @brief Destroy the MainController object, this destructor is defaulted
     * as we are using smart pointers to manage the resources of the main
     * controller, and we don't have any special cleanup logic to implement.
     *
     */
    MainController::~MainController() = default;

    /**
     * @brief Start the main controller, this function shows the main window
     * and ensures that a profile exists by using the EnsureProfileController,
     * this is typically called after the main controller has been initialized
     * to start the application and display the UI to the user.
     */
    void MainController::start()
    {
        _impl->_mainWindow.show();

        auto controller = controller::EnsureProfileController{
            _impl->_mainWindow,
            _impl->_storeContainer,
            _impl->_undoStack,
            _impl->_settings
        };

        controller.ensureProfileExists();

        _impl->_sideBarController.refresh();

        _impl->_updateCheckService.start();
    }

}   // namespace controller