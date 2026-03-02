#ifndef __LOGGING__LOG_CATEGORIES_HPP__
#define __LOGGING__LOG_CATEGORIES_HPP__

#include <set>
#include <string>
#include <unordered_map>
#include <vector>

#include "log_category.hpp"

namespace logging
{
    /**
     * @brief Struct representing a collection of log categories, organized in a
     * hierarchical structure.
     *
     */
    struct LogCategories
    {
       public:
        std::vector<LogCategory>                       _categories;
        std::unordered_map<std::string, LogCategoryId> _categoryNameToIdMap;

       public:
        LogCategories() = default;

        explicit LogCategories(
            const std::set<std::string>& categoryNames,
            const LogLevel&              defaultLogLevel
        );

        LogCategoryId findLogCategory(const std::string& categoryName) const;
        std::vector<LogCategory>   getCategories() const;
        std::optional<LogCategory> getCategory(
            const std::string& categoryName
        ) const;

       private:
        [[nodiscard]] LogCategoryId _addLogCategory(
            const std::string& fullName,
            const LogLevel&    logLevel
        );

        [[nodiscard]] LogCategoryId _addLogCategory(
            const LogCategoryId& parentCategoryId,
            const std::string&   segment,
            const std::string&   fullName,
            const LogLevel&      logLevel
        );

        [[nodiscard]] LogCategoryId _getOrCreateLogCategory(
            LogCategoryId      parentCategoryId,
            const std::string& segment,
            const std::string& fullName,
            const LogLevel&    logLevel
        );

        static void _appendSegmentToFullName(
            std::string&       parentFullName,
            const std::string& segment
        );

        template <typename Fn>
        void _forEachSegment(const std::string& fullName, Fn&& func) const
        {
            std::size_t pos = 0;
            while (pos < fullName.size())
            {
                const auto dot = fullName.find('.', pos);
                const auto end =
                    (dot == std::string::npos) ? fullName.size() : dot;

                std::forward<Fn>(func)(fullName.substr(pos, end - pos));

                pos = end + 1;
            }
        }
    };

}   // namespace logging

#endif   // __LOGGING__LOG_CATEGORIES_HPP__