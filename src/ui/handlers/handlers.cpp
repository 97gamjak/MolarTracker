#include "handlers.hpp"

#include "settings/settings.hpp"

namespace ui
{
    /**
     * @brief Construct a new Handlers:: Handlers object
     *
     * @param settings
     */
    Handlers::Handlers(settings::Settings& settings)
        : _loggingHandler(settings.getLoggingSettings())
    {
    }

    /**
     * @brief get the dirty state handler
     *
     * @return DirtyStateHandler&
     */
    [[nodiscard]] DirtyStateHandler& Handlers::getDirtyStateHandler()
    {
        return _dirtyStateHandler;
    }

    /**
     * @brief get the dirty state handler (const version)
     *
     * @return const DirtyStateHandler&
     */
    [[nodiscard]] const DirtyStateHandler& Handlers::getDirtyStateHandler(
    ) const
    {
        return _dirtyStateHandler;
    }

}   // namespace ui