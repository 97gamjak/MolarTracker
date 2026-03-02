#ifndef __LOGGING__LOG_CATEGORY_REGISTRY_HPP__
#define __LOGGING__LOG_CATEGORY_REGISTRY_HPP__

#include <vector>

#include "log_category.hpp"

namespace logging
{
    /**
     * @brief A registry for log categories, implemented as a singleton
     *
     */
    class LogCategoryRegistry
    {
       private:
        LogCategory _rootCategory;

       public:
        static LogCategoryRegistry& getInstance();

        void initialize();

       private:
        LogCategoryRegistry() = default;
    };

}   // namespace logging

#endif   // __LOGGING__LOG_CATEGORY_REGISTRY_HPP__