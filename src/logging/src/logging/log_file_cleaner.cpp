#include "logging/log_file_cleaner.hpp"

#include <chrono>
#include <filesystem>
#include <string>
#include <system_error>

namespace logging
{

    /**
     * @brief Delete log files in `directory` older than `maxAgeDays` days
     *
     * Only regular files whose name starts with `prefix` and ends with
     * `suffix` are considered. Symlinks and other non-regular entries are
     * skipped. If `maxAgeDays` is 0, the function returns immediately.
     * Individual file errors are suppressed via error_code overloads so that
     * a single unreadable file does not abort the whole cleanup pass.
     */
    void LogFileCleaner::cleanByAge(
        const std::filesystem::path& directory,
        const std::string&           prefix,
        const std::string&           suffix,
        std::size_t                  maxAgeDays
    )
    {
        if (maxAgeDays == 0)
            return;

        std::error_code ec;
        if (!std::filesystem::is_directory(directory, ec) || ec)
            return;

        const auto now = std::filesystem::file_time_type::clock::now();
        const auto maxAge =
            std::chrono::hours(24) * static_cast<long long>(maxAgeDays);
        const auto cutoff = now - maxAge;

        for (const auto& entry :
             std::filesystem::directory_iterator(directory, ec))
        {
            if (ec)
                break;

            if (!entry.is_regular_file(ec) || ec)
            {
                ec.clear();
                continue;
            }

            const auto filename = entry.path().filename().string();

            if (!filename.starts_with(prefix) || !filename.ends_with(suffix))
                continue;

            const auto mtime = entry.last_write_time(ec);
            if (ec)
            {
                ec.clear();
                continue;
            }

            if (mtime < cutoff)
                std::filesystem::remove(entry.path(), ec);

            ec.clear();
        }
    }

}   // namespace logging
