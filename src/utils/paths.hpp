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

enum class DirKind
{
    Config,   // settings, preferences
    Data,     // persistent data (SQLite DB, user-created content)
};

enum class EnvError
{
    NotFound,
    Empty
};

using PathResult = std::expected<std::filesystem::path, EnvError>;

PathResult _linux_home();

PathResult _linux_xdg(
    std::string_view env_name,
    std::string_view fallback_suffix
);

#if defined(_WIN32)
std::filesystem::path _win_known_folder(REFKNOWNFOLDERID id);
#endif

std::filesystem::path user_dir(DirKind kind, std::string_view app_name);

std::filesystem::path ensure_dir(const std::filesystem::path& p);

std::filesystem::path config_dir(std::string_view app_name);
std::filesystem::path data_dir(std::string_view app_name);

#endif   // __UTILS__PATHS_HPP__