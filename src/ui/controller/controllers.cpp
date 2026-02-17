#include "controllers.hpp"

#include "settings/settings.hpp"

namespace ui
{
    /**
     * @brief Construct a new Controllers:: Controllers object
     *
     * @param settings
     */
    Controllers::Controllers(settings::Settings& settings)
        : _loggingController(settings.getLoggingSettings())
    {
    }

}   // namespace ui