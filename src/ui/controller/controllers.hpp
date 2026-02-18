#ifndef __UI__CONTROLLER__CONTROLLERS_HPP__
#define __UI__CONTROLLER__CONTROLLERS_HPP__

#include "logging_controller.hpp"

namespace settings
{
    class Settings;   // Forward declaration
}

namespace ui
{
    class Controllers
    {
       private:
        LoggingController _loggingController;

       public:
        Controllers() = delete;
        explicit Controllers(settings::Settings& settings);
    };
}   // namespace ui

#endif   // __UI__CONTROLLER__CONTROLLERS_HPP__