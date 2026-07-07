#ifndef __LOGGING__INCLUDE__LOGGING__LOG_FILE_CLEANER_HPP__
#define __LOGGING__INCLUDE__LOGGING__LOG_FILE_CLEANER_HPP__

#include <cstddef>
#include <filesystem>
#include <string>

namespace logging
{

    /**
     * @brief Utility for age-based log file cleanup
     *
     * Deletes log files in a directory that are older than a configured
     * number of days. Intended to be called at application startup to
     * prevent unbounded accumulation of session log files.
     */
    class LogFileCleaner
    {
       public:
        /**
         * @brief Delete log files older than maxAgeDays days
         *
         * Scans `directory` (non-recursively) for regular files whose name
         * starts with `prefix` and ends with `suffix`, and deletes those
         * whose last-write-time is older than `maxAgeDays` days.
         * A `maxAgeDays` of 0 disables cleanup entirely (no-op).
         * Errors on individual files are suppressed — cleanup is best-effort.
         *
         * @param directory  Directory to scan
         * @param prefix     Required filename prefix (e.g. "molar_tracker_")
         * @param suffix     Required filename suffix/extension (e.g. ".log")
         * @param maxAgeDays Maximum file age in days; 0 disables cleanup
         */
        static void cleanByAge(
            const std::filesystem::path& directory,
            const std::string&           prefix,
            const std::string&           suffix,
            std::size_t                  maxAgeDays
        );
    };

}   // namespace logging

#endif   // __LOGGING__INCLUDE__LOGGING__LOG_FILE_CLEANER_HPP__
