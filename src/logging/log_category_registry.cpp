#include "log_category_registry.hpp"

#include "log_categories.gen.hpp"

namespace logging
{

    /**
     * @brief Get the singleton instance of LogCategoryRegistry
     *
     * @return LogCategoryRegistry&
     */
    LogCategoryRegistry& LogCategoryRegistry::getInstance()
    {
        static LogCategoryRegistry instance;
        return instance;
    }

    /**
     * @brief Initialize the log category registry with categories from the
     * generated log categories file
     *
     */
    void LogCategoryRegistry::initialize()
    {
        for (const auto& category : CategoryRegistry::getInstance().categories)
        {
            _rootCategory.addCategory(category, LogLevel::Info);
        }
    }

}   // namespace logging