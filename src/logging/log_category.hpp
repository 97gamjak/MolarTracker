#ifndef __LOGGING__LOG_CATEGORY_HPP__
#define __LOGGING__LOG_CATEGORY_HPP__

#include <string>
#include <vector>

#include "config/logging_base.hpp"

namespace logging
{
    using LogCategoryId                                 = std::int64_t;
    inline constexpr LogCategoryId InvalidLogCategoryId = -1;
    inline constexpr LogCategoryId RootLogCategoryId    = 0;

    /**
     * @brief Class representing a single log category in the logging system,
     * which can have a hierarchical structure with parent and child categories
     *
     */
    class LogCategory
    {
       private:
        /// The unique identifier for the log category
        LogCategoryId _id{InvalidLogCategoryId};

        /// The identifier of the parent log category, or InvalidLogCategoryId
        /// if this is a root category
        LogCategoryId _parentId{InvalidLogCategoryId};

        /// The segment name of the log category (the last part of the full
        /// name)
        std::string _segment;

        /// The full name of the log category, including all segments separated
        /// by dots
        std::string _fullName;

        /// The log level associated with this category
        LogLevel _logLevel;

        /// The identifiers of the child log categories
        std::vector<LogCategoryId> _children;

       public:
        static LogCategory createRootCategory(LogLevel logLevel);

        explicit LogCategory(
            LogCategoryId id,
            LogCategoryId parentId,
            std::string   segment,
            std::string   fullName,
            LogLevel      logLevel
        );

        void addSubCategory(LogCategoryId subCategoryId);

        void                   setLogLevel(const LogLevel& logLevel);
        [[nodiscard]] LogLevel getLogLevel() const;

        [[nodiscard]] std::string getName() const;
        [[nodiscard]] std::string getSegment() const;

        [[nodiscard]] LogCategoryId getId() const;
        [[nodiscard]] LogCategoryId getParentId() const;

        [[nodiscard]] std::vector<LogCategoryId> getChildren() const;

        [[nodiscard]] bool hasChildren() const;
    };
}   // namespace logging

#endif   // __LOGGING__LOG_CATEGORY_HPP__