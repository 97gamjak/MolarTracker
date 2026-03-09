#include "utils/paths.hpp"

#if defined(_WIN32)
#include <algorithm>
#include <cctype>
#include <string>
#endif

#include <algorithm>

namespace utils
{

    // TODO(97gamjak): migrate this to mstd later on
    // https://97gamjak.atlassian.net/browse/MOLTRACK-90
    enum class EnvError : std::uint8_t
    {
        NotFound,
        Empty
    };

    using PathResult = std::expected<std::filesystem::path, EnvError>;

#if defined(_WIN32)
    /**
     * @brief Normalize a Windows filename for reserved name checking.
     *
     * This function converts the input string @p s to uppercase and trims
     * trailing spaces and dots, following Windows filename normalization rules.
     *
     * @param s The input string to normalize.
     * @return std::string The normalized string.
     */
    inline std::string _winNormalizeName(std::string_view s)
    {
        // Trim trailing spaces and dots (Windows behavior)
        while (!s.empty() && (s.back() == ' ' || s.back() == '.'))
            s.remove_suffix(1);

        std::string out(s);
        std::transform(
            out.begin(),
            out.end(),
            out.begin(),
            [](unsigned char c) { return static_cast<char>(std::toupper(c)); }
        );
        return out;
    }
#endif

#if defined(_WIN32)
    /**
     * @brief Check if a string is a Windows reserved name.
     *
     * @param s The input string to check.
     * @return true If the string is a reserved name.
     * @return false If the string is not a reserved name.
     */
    inline bool _isWindowsReservedName(std::string_view s)
    {
        const std::string name = _winNormalizeName(s);

        if (name == "CON" || name == "PRN" || name == "AUX" || name == "NUL")
            return true;

        if (name.size() == 4)
        {
            if (name.starts_with("COM") || name.starts_with("LPT"))
            {
                const char d = name[3];
                return d >= '1' && d <= '9';
            }
        }

        return false;
    }
#endif

    namespace
    {
        /**
         * @brief Validate that an application name is safe for use in paths.
         *
         * This function checks that the provided string @p s is a valid
         * application name that can be safely used as a directory name under
         * user config/data directories. It performs checks to disallow control
         * characters, path separators, Windows reserved characters, and path
         * traversal patterns.
         *
         * @param s The application name to validate.
         * @return true If the application name is safe.
         * @return false If the application name is unsafe.
         */
        bool isSafeAppName(std::string_view appName) noexcept
        {
            if (appName.empty())
                return false;

            // Disallow leading/trailing spaces (optional, but avoids weird
            // folders)
            if (appName.front() == ' ' || appName.back() == ' ')
                return false;

            auto isCharInvalid = [](unsigned char character)
            {
                // Control chars
                constexpr unsigned char controlChar1 =
                    0x20;   // space is the last allowed control char
                constexpr unsigned char controlChar2 =
                    0x7F;   // DEL char, also disallowed

                if (character < controlChar1 || character == controlChar2)
                    return true;

                // Path separators / traversal enablers
                if (character == '/' || character == '\\')
                    return true;

                // Windows reserved characters (also safe to ban on Linux)
                if (character == '<' || character == '>' || character == ':' ||
                    character == '"' || character == '|' || character == '?' ||
                    character == '*')
                    return true;

                return false;
            };

            if (std::ranges::any_of(appName, isCharInvalid))
                return false;

            // Disallow "." or ".." exactly
            if (appName == "." || appName == "..")
                return false;

            // Disallow segments like "a/..", "a\.." already blocked by
            // separators, but also reject any occurrence of ".." to be
            // conservative:
            if (appName.find("..") != std::string_view::npos)
                return false;

#if defined(_WIN32)
            if (_isWindowsReservedName(appName))
                return false;
#endif

            return true;
        }
    }   // namespace

    namespace
    {
        /**
         * @brief Retrieve the current user's home directory from the
         * environment.
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
         * @return PathResult Either a valid std::filesystem::path on success or
         * an unexpected EnvError on failure.
         */
        PathResult unixHome()
        {
            // NOLINTNEXTLINE(concurrency-mt-unsafe)
            const auto* home = std::getenv("HOME");

            if (home == nullptr)
                return std::unexpected(EnvError::NotFound);

            if (*home == 0)
                return std::unexpected(EnvError::Empty);

            return std::filesystem::path(home);
        }
    }   // namespace

    namespace
    {
        /**
         * @brief Resolve an XDG-style directory using an environment variable
         * with a fallback under $HOME.
         *
         * This function attempts to read an environment variable specified by
         * @p env_name. If that variable is set and non-empty, its value is
         * returned as a std::filesystem::path wrapped in PathResult. If the
         * environment variable is missing or empty, the function falls back to
         * using the user's home directory (via unix_home) and appends @p
         * fallback_suffix to it (e.g.
         * ".config" or
         * ".local/share").
         *
         * Notes:
         * - The @p env_name parameter should be the name of an environment
         * variable (for example "XDG_CONFIG_HOME").
         * - The @p fallback_suffix should be a relative path component to
         * append to $HOME when the env var is unset.
         *
         * @param env_name Name of the environment variable to consult (e.g.
         * "XDG_CONFIG_HOME").
         * @param fallback_suffix Path suffix to append to $HOME when the
         * environment variable is missing (e.g. ".config").
         * @return PathResult Either a resolved std::filesystem::path on success
         * or an unexpected EnvError propagated from unix_home.
         */
        PathResult unixXdg(
            std::string_view env_name,
            std::string_view fallback_suffix
        )
        {
            // NOLINTNEXTLINE(concurrency-mt-unsafe)
            const auto* env_path = std::getenv(std::string(env_name).c_str());

            if ((env_path != nullptr) && (*env_path != 0))
                return std::filesystem::path(env_path);

            const auto home = unixHome();

            if (!home)
                return home;

            return home.value() / std::filesystem::path(fallback_suffix);
        }
    }   // namespace

#if defined(_WIN32)
    /**
     * @brief Query a Windows Known Folder and return it as a
     * std::filesystem::path.
     *
     * This convenience wrapper calls SHGetKnownFolderPath for the provided
     * Known Folder ID and converts the returned PWSTR into a
     * std::filesystem::path. Allocated memory is freed via CoTaskMemFree.
     *
     * Behavior:
     * - On success returns the folder path.
     * - On failure (HRESULT indicates failure or returned pointer is null)
     * returns an empty std::filesystem::path.
     *
     * @note This function is only available on Windows builds.
     *
     * @param id The REFKNOWNFOLDERID to query (e.g.
     * FOLDERID_RoamingAppData).
     * @return std::filesystem::path The resolved folder path, or an empty
     * path on error.
     */
    std::filesystem::path winKnownFolder(REFKNOWNFOLDERID id)
    {
        PWSTR      wide = nullptr;
        const auto hr =
            SHGetKnownFolderPath(id, KF_FLAG_DEFAULT, nullptr, &wide);

        if (FAILED(hr) || wide == nullptr)
            return std::filesystem::path();

        std::filesystem::path p(wide);
        CoTaskMemFree(wide);
        return p;
    }
#endif

    /**
     * @brief Compute the per-user application directory for the requested
     * kind.
     *
     * On Windows:
     * - Both Config and Data currently map to the roaming AppData known
     * folder (FOLDERID_RoamingAppData).
     * - If resolution fails, std::unexpected(PathError::Empty) is returned.
     *
     * On POSIX (Linux/UNIX):
     * - For DirKind::Config, consult XDG_CONFIG_HOME with fallback
     * "$HOME/.config".
     * - For DirKind::Data, consult XDG_DATA_HOME with fallback
     * "$HOME/.local/share".
     * - If the base directory cannot be resolved (e.g. missing HOME),
     *   std::unexpected(PathError::Empty) is returned.
     *
     * The returned path is the base user directory joined with @p app_name
     * (i.e. base/app_name).
     *
     * @param kind Which kind of per-user directory to resolve (Config or
     * Data).
     * @param app_name Name of the application subdirectory to append to the
     * base directory.
     * @return PathErrorResult The resolved per-user application directory
     * path or an PathError on failure.
     */
    PathErrorResult userDir(DirKind kind, std::string_view app_name)
    {
        if (!isSafeAppName(app_name))
            return std::unexpected(PathError::InvalidAppName);

#if defined(_WIN32)
        std::filesystem::path base = std::filesystem::path();

        switch (kind)
        {
            case DirKind::Config:
            case DirKind::Data:
                base = winKnownFolder(FOLDERID_RoamingAppData);
                break;
        }
        if (base.empty())
            return std::unexpected(PathError::Empty);

        return base / app_name;
#else
        PathResult base;
        switch (kind)
        {
            case DirKind::Config:
                base = unixXdg("XDG_CONFIG_HOME", ".config");
                break;
            case DirKind::Data:
                base = unixXdg("XDG_DATA_HOME", ".local/share");
                break;
        }
        if (!base)
            return std::unexpected(PathError::Empty);

        return base.value() / app_name;
#endif
    }

    /**
     * @brief Ensure that a directory exists, creating any missing parent
     * directories.
     *
     * If @p path is empty this function returns PathError::Empty. When @p
     * path is non-empty, std::filesystem::create_directories is invoked
     * with a non-throwing overload that accepts a std::error_code; on
     * failure this function returns PathError::FailedCreate and on success
     * it returns @p path.
     *
     * @param path The directory path to ensure exists.
     * @return PathErrorResult The ensured directory path, or a PathError on
     * failure.
     */
    PathErrorResult ensureDir(const std::filesystem::path& path)
    {
        if (path.empty())
            return std::unexpected(PathError::Empty);

        std::error_code errorCode;
        std::filesystem::create_directories(path, errorCode);

        if (errorCode)
            return std::unexpected(PathError::FailedCreate);

        // Verify that the resulting path exists and is a directory. This
        // catches cases where the path exists but is not a directory, or
        // where a race or permission issue prevented the directory from
        // being usable.
        std::error_code status_ec;
        const auto      status = std::filesystem::status(path, status_ec);
        if (status_ec || !std::filesystem::is_directory(status))
            return std::unexpected(PathError::FailedCreate);
        return path;
    }

    /**
     * @brief Return the per-user configuration directory for the given
     * application, creating it if necessary.
     *
     * This is a small convenience wrapper that resolves the application
     * config directory via user_dir(DirKind::Config, app_name) and then
     * calls ensure_dir to create it on disk if it does not yet exist.
     *
     * @param app_name Name of the application subdirectory.
     * @return PathErrorResult The ensured config directory path, or an
     * PathError on failure.
     */
    PathErrorResult configDir(std::string_view app_name)
    {
        const auto dir = userDir(DirKind::Config, app_name);

        if (!dir)
            return dir;

        return ensureDir(dir.value());
    }

    /**
     * @brief Return the per-user data directory for the given application,
     * creating it if necessary.
     *
     * This is a small convenience wrapper that resolves the application
     * data directory via user_dir(DirKind::Data, app_name) and then calls
     * ensure_dir to create it on disk if it does not yet exist.
     *
     * @param app_name Name of the application subdirectory.
     * @return PathErrorResult The ensured data directory path, or an
     * PathError on failure.
     */
    PathErrorResult dataDir(std::string_view app_name)
    {
        const auto dir = userDir(DirKind::Data, app_name);

        if (!dir)
            return dir;

        return ensureDir(dir.value());
    }

}   // namespace utils