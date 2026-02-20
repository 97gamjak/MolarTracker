#ifndef __UI__CONTROLLER__CONTROLLERS_HPP__
#define __UI__CONTROLLER__CONTROLLERS_HPP__

#include "logging_controller.hpp"

namespace settings
{
    class Settings;   // Forward declaration
}

namespace ui
{
    /**
     * @brief A container for all controllers in the application. This class is
     * responsible for initializing and managing the lifecycle of all
     * controllers.
     *
     */
    class Controllers
    {
       private:
        /// Controller for handling logging related functionality
        LoggingController _loggingController;

       public:
        Controllers() = delete;
        explicit Controllers(settings::Settings& settings);
    };
}   // namespace ui

#endif   // __UI__CONTROLLER__CONTROLLERS_HPP__