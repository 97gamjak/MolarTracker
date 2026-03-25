#ifndef __UTILS__PATHS_HPP__
#define __UTILS__PATHS_HPP__

#include <cstdint>
#include <expected>
#include <filesystem>
#include <string_view>

#if defined(_WIN32)
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <ShlObj.h>
#include <Windows.h>
#endif

namespace utils
{

    /**
     * @brief Kinds of user directories
     *
     */
    enum class DirKind : std::uint8_t
    {
        Config,   // settings, preferences
        Data,     // persistent data (SQLite DB, user-created content)
    };

    /**
     * @brief Errors that can occur when resolving or creating paths
     *
     */
    enum class PathError : std::uint8_t
    {
        Empty,
        InvalidAppName,
        FailedCreate,
    };

    using PathErrorResult = std::expected<std::filesystem::path, PathError>;

    PathErrorResult userDir(DirKind kind, std::string_view app_name);

    PathErrorResult ensureDir(const std::filesystem::path& path);

    PathErrorResult configDir(std::string_view app_name);
    PathErrorResult dataDir(std::string_view app_name);

}   // namespace utils

#endif   // __UTILS__PATHS_HPP__