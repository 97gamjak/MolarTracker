#include "utils/paths.hpp"

// TODO: migrate this to mstd later on
enum class EnvError
{
    NotFound,
    Empty
};

using PathResult = std::expected<std::filesystem::path, EnvError>;

/**
 * @brief Validate that an application name is safe for use in paths.
 *
 * This function checks that the provided string @p s is a valid application
 * name that can be safely used as a directory name under user config/data
 * directories. It performs checks to disallow control characters, path
 * separators, Windows reserved characters, and path traversal patterns.
 *
 * @param s The application name to validate.
 * @return true If the application name is safe.
 * @return false If the application name is unsafe.
 */
bool is_safe_app_name(std::string_view s) noexcept
{
    if (s.empty())
        return false;

    // Disallow leading/trailing spaces (optional, but avoids weird folders)
    if (s.front() == ' ' || s.back() == ' ')
        return false;

    for (const auto& ch : s)
    {
        // control chars
        if (ch < 0x20 || ch == 0x7F)
            return false;

        // path separators / traversal enablers
        if (ch == '/' || ch == '\\')
            return false;

        // Windows reserved characters (also safe to ban on Linux)
        if (ch == '<' || ch == '>' || ch == ':' || ch == '"' || ch == '|' ||
            ch == '?' || ch == '*')
            return false;

        // NUL
        if (ch == '\0')
            return false;
    }

    // Disallow "." or ".." exactly
    if (s == "." || s == "..")
        return false;

    // Disallow segments like "a/..", "a\.." already blocked by separators,
    // but also reject any occurrence of ".." to be conservative:
    if (s.find("..") != std::string_view::npos)
        return false;

    return true;
}

/**
 * @brief Retrieve the current user's home directory from the environment.
 *
 * This helper reads the HOME environment variable and converts it to a
 * std::filesystem::path wrapped in the project's PathResult type.
 *
 * Behavior:
 * - If the HOME environment variable is not present, returns
 * std::unexpected(EnvError::NotFound).
 * - If the HOME environment variable is present but empty, returns
 * std::unexpected(EnvError::Empty).
 * - Otherwise returns the path constructed from HOME.
 *
 * @return PathResult Either a valid std::filesystem::path on success or an
 * unexpected EnvError on failure.
 */
PathResult linux_home()
{
    const auto* home = std::getenv("HOME");

    if (!home)
        return std::unexpected(EnvError::NotFound);

    if (!*home)
        return std::unexpected(EnvError::Empty);

    return std::filesystem::path(home);
}

/**
 * @brief Resolve an XDG-style directory using an environment variable with a
 * fallback under $HOME.
 *
 * This function attempts to read an environment variable specified by @p
 * env_name. If that variable is set and non-empty, its value is returned as a
 * std::filesystem::path wrapped in PathResult. If the environment variable is
 * missing or empty, the function falls back to using the user's home directory
 * (via linux_home) and appends @p fallback_suffix to it (e.g. ".config" or
 * ".local/share").
 *
 * Notes:
 * - The @p env_name parameter should be the name of an environment variable
 * (for example "XDG_CONFIG_HOME").
 * - The @p fallback_suffix should be a relative path component to append to
 * $HOME when the env var is unset.
 *
 * @param env_name Name of the environment variable to consult (e.g.
 * "XDG_CONFIG_HOME").
 * @param fallback_suffix Path suffix to append to $HOME when the environment
 * variable is missing (e.g. ".config").
 * @return PathResult Either a resolved std::filesystem::path on success or an
 * unexpected EnvError propagated from linux_home.
 */
PathResult linux_xdg(
    std::string_view env_name,
    std::string_view fallback_suffix
)
{
    const auto* env_path = std::getenv(std::string(env_name).c_str());

    if (env_path && *env_path)
        return std::filesystem::path(env_path);

    const auto home = linux_home();

    if (!home)
        return home;

    return home.value() / std::filesystem::path(fallback_suffix);
}

#if defined(_WIN32)
/**
 * @brief Query a Windows Known Folder and return it as a std::filesystem::path.
 *
 * This convenience wrapper calls SHGetKnownFolderPath for the provided Known
 * Folder ID and converts the returned PWSTR into a std::filesystem::path.
 * Allocated memory is freed via CoTaskMemFree.
 *
 * Behavior:
 * - On success returns the folder path.
 * - On failure (HRESULT indicates failure or returned pointer is null) returns
 * an empty std::filesystem::path.
 *
 * @note This function is only available on Windows builds.
 *
 * @param id The REFKNOWNFOLDERID to query (e.g. FOLDERID_RoamingAppData).
 * @return std::filesystem::path The resolved folder path, or an empty path on
 * error.
 */
std::filesystem::path win_known_folder(REFKNOWNFOLDERID id)
{
    PWSTR      wide = nullptr;
    const auto hr   = SHGetKnownFolderPath(id, KF_FLAG_DEFAULT, nullptr, &wide);

    if (FAILED(hr) || wide == nullptr)
        return std::filesystem::path();

    std::filesystem::path p(wide);
    CoTaskMemFree(wide);
    return p;
}
#endif

/**
 * @brief Compute the per-user application directory for the requested kind.
 *
 * On Windows:
 * - Both Config and Data currently map to the roaming AppData known folder
 * (FOLDERID_RoamingAppData).
 * - If resolution fails, an empty std::filesystem::path is returned.
 *
 * On POSIX (Linux/UNIX):
 * - For DirKind::Config, consult XDG_CONFIG_HOME with fallback "$HOME/.config".
 * - For DirKind::Data, consult XDG_DATA_HOME with fallback
 * "$HOME/.local/share".
 * - If the base directory cannot be resolved (e.g. missing HOME), an empty
 * std::filesystem::path is returned.
 *
 * The returned path is the base user directory joined with @p app_name (i.e.
 * base/app_name).
 *
 * @param kind Which kind of per-user directory to resolve (Config or Data).
 * @param app_name Name of the application subdirectory to append to the base
 * directory.
 * @return PathErrorResult The resolved per-user application directory path or
 * an PathError on failure.
 */
PathErrorResult user_dir(DirKind kind, std::string_view app_name)
{
    if (!is_safe_app_name(app_name))
        return std::unexpected(PathError::InvalidAppName);

#if defined(_WIN32)
    std::filesystem::path base = std::filesystem::path();

    switch (kind)
    {
        case DirKind::Config:
        case DirKind::Data:
            base = win_known_folder(FOLDERID_RoamingAppData);
            break;
    }
    if (base.empty())
        return std::unexpected(PathError::Empty);

    return base / std::filesystem::path(app_name);
#else
    PathResult base;
    switch (kind)
    {
        case DirKind::Config:
            base = linux_xdg("XDG_CONFIG_HOME", ".config");
            break;
        case DirKind::Data:
            base = linux_xdg("XDG_DATA_HOME", ".local/share");
            break;
    }
    if (!base)
        return std::unexpected(PathError::Empty);

    return base.value() / std::filesystem::path(app_name);
#endif
}

/**
 * @brief Ensure that a directory exists, creating any missing parent
 * directories.
 *
 * If @p path is empty this function is a no-op and simply returns the empty
 * path. When @p path is non-empty, std::filesystem::create_directories is
 * invoked with a non-throwing overload that accepts a std::error_code; any
 * creation errors are intentionally ignored here.
 *
 * @param path The directory path to ensure exists.
 * @return PathErrorResult The ensured directory path, or an PathError on
 * failure.
 */
PathErrorResult ensure_dir(const std::filesystem::path& path)
{
    if (path.empty())
        return std::unexpected(PathError::Empty);

    std::error_code ec;
    std::filesystem::create_directories(path, ec);

    // TODO: distinguish failure reasons?
    if (ec)
        return std::unexpected(PathError::FailedCreate);

    return path;
}

/**
 * @brief Return the per-user configuration directory for the given application,
 * creating it if necessary.
 *
 * This is a small convenience wrapper that resolves the application config
 * directory via user_dir(DirKind::Config, app_name) and then calls ensure_dir
 * to create it on disk if it does not yet exist.
 *
 * @param app_name Name of the application subdirectory.
 * @return PathErrorResult The ensured config directory path, or an PathError on
 * failure.
 */
PathErrorResult config_dir(std::string_view app_name)
{
    const auto dir = user_dir(DirKind::Config, app_name);

    if (!dir)
        return dir;

    return ensure_dir(dir.value());
}

/**
 * @brief Return the per-user data directory for the given application, creating
 * it if necessary.
 *
 * This is a small convenience wrapper that resolves the application data
 * directory via user_dir(DirKind::Data, app_name) and then calls ensure_dir to
 * create it on disk if it does not yet exist.
 *
 * @param app_name Name of the application subdirectory.
 * @return PathErrorResult The ensured data directory path, or an PathError on
 * failure.
 */
PathErrorResult data_dir(std::string_view app_name)
{
    const auto dir = user_dir(DirKind::Data, app_name);

    if (!dir)
        return dir;

    return ensure_dir(dir.value());
}