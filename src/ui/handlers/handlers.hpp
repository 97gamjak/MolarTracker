#ifndef __UI__HANDLERS__HANDLERS_HPP__
#define __UI__HANDLERS__HANDLERS_HPP__

#include "dirty_handler.hpp"
#include "logging_handler.hpp"

namespace settings
{
    class Settings;   // Forward declaration
}   // namespace settings

namespace ui
{
    /**
     * @brief A container for all handlers in the application. This class is
     * responsible for initializing and managing the lifecycle of all
     * handlers.
     *
     */
    class Handlers
    {
       private:
        /// Handler for handling logging related functionality
        LoggingHandler _loggingHandler;

        /// Handler for managing the dirty state of the application
        DirtyStateHandler _dirtyStateHandler;

       public:
        Handlers() = delete;
        explicit Handlers(settings::Settings& settings);

        [[nodiscard]] DirtyStateHandler&       getDirtyStateHandler();
        [[nodiscard]] const DirtyStateHandler& getDirtyStateHandler() const;
    };
}   // namespace ui

#endif   // __UI__HANDLERS__HANDLERS_HPP__