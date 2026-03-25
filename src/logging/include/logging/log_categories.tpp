#ifndef __LOGGING__INCLUDE__LOGGING__LOG_CATEGORIES_TPP__
#define __LOGGING__INCLUDE__LOGGING__LOG_CATEGORIES_TPP__

#include "log_categories.hpp"

namespace logging
{

    /**
     * @brief Helper function to iterate over each segment of a category name
     * and apply a given function to it.
     *
     * @tparam Fn
     * @param fullName
     * @param func
     */
    template <typename Fn>
    void LogCategories::_forEachSegment(const std::string& fullName, Fn&& func)
    {
        std::size_t pos = 0;

        while (pos < fullName.size())
        {
            const auto dot = fullName.find('.', pos);
            const auto end = (dot == std::string::npos) ? fullName.size() : dot;

            std::forward<Fn>(func)(fullName.substr(pos, end - pos));

            pos = end + 1;
        }
    }

}   // namespace logging

#endif   // __LOGGING__INCLUDE__LOGGING__LOG_CATEGORIES_TPP__