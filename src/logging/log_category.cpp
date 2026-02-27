#include "log_category.hpp"

#include "log_categories.gen.hpp"

namespace logging
{
    /**
     * @brief Construct a new Log Category:: Log Category object
     *
     * @param name
     */
    LogCategory::LogCategory(const char* name) : _name{name}
    {
        CategoryRegistry::instance().intern(name);
    }

    /**
     * @brief Get the name of the log category
     *
     * @return std::string
     */
    std::string LogCategory::getName() const { return _name; }
}   // namespace logging