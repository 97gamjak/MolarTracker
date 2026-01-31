#ifndef __UTILS__PATHS_HPP__
#define __UTILS__PATHS_HPP__

#include <cstdlib>
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
    enum class DirKind
    {
        Config,   // settings, preferences
        Data,     // persistent data (SQLite DB, user-created content)
    };

    /**
     * @brief Errors that can occur when resolving or creating paths
     *
     */
    enum class PathError
    {
        Empty,
        InvalidAppName,
        FailedCreate,
    };

    using PathErrorResult = std::expected<std::filesystem::path, PathError>;

    PathErrorResult user_dir(DirKind kind, std::string_view app_name);

    PathErrorResult ensure_dir(const std::filesystem::path& path);

    PathErrorResult config_dir(std::string_view app_name);
    PathErrorResult data_dir(std::string_view app_name);

}   // namespace utils

#endif   // __UTILS__PATHS_HPP__