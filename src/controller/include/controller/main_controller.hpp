#ifndef __CONTROLLER__INCLUDE__CONTROLLER__MAIN_CONTROLLER_HPP__
#define __CONTROLLER__INCLUDE__CONTROLLER__MAIN_CONTROLLER_HPP__

#include <memory>

namespace controller
{
    /**
     * @brief The main controller of the application, this class is responsible
     * for initializing and managing the different components of the
     * application, such as the settings, the profile store, the command stack,
     * and the main window. It also serves as a central point for coordinating
     * actions between these components, for example when a command is executed,
     * it can notify the main controller to update the UI or save the settings.
     *
     */
    class MainController
    {
       private:
        struct Impl;
        /// Pimpl idiom -- to hide impl details and reduce compile-time
        /// dependencies
        std::unique_ptr<Impl> _impl;

       public:
        explicit MainController();
        ~MainController();

        void start();
    };

}   // namespace controller

#endif   // __CONTROLLER__INCLUDE__CONTROLLER__MAIN_CONTROLLER_HPP__