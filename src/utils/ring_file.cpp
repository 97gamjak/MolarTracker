#include "utils/ring_file.hpp"

#include <filesystem>
#include <format>
#include <iostream>

/**
 * @brief Constructs a RingFile with the given configuration
 *
 * @note The directory specified in the configuration will be created if it
 * does not exist, and the current log file will be created/opened during
 * construction.
 *
 * @param config Configuration for the RingFile
 */
RingFile::RingFile(Config config) : _config(std::move(config))
{
    _normalizeConfig();
    std::filesystem::create_directories(_config.directory);
    _openCurrent();
}

/**
 * @brief Destructor for RingFile
 *
 * @note Flushes and closes the file if open, unless writing to std::cerr.
 */
RingFile::~RingFile()
{
    // if we don't have a file we decided to write to std::cerr
    if (_file)
    {
        flush();
        close();
    }
}

/**
 * @brief Move constructor for RingFile
 *
 * @param other The RingFile to move from
 */
RingFile::RingFile(RingFile&& other) noexcept { *this = std::move(other); }

/**
 * @brief Move assignment operator for RingFile
 *
 * @param other The RingFile to move from
 * @return RingFile& Reference to this RingFile
 */
RingFile& RingFile::operator=(RingFile&& other) noexcept
{
    if (this == &other)
        return *this;

    close();
    _config             = std::move(other._config);
    _file               = std::move(other._file);
    _initialFileSize    = other._initialFileSize;
    _bytesWritten       = other._bytesWritten;
    _currentSymlinkPath = std::move(other._currentSymlinkPath);

    other._initialFileSize = 0;
    other._bytesWritten    = 0;
    return *this;
}

/**
 * @brief Writes a line to the ring file
 *
 * @note A newline character is appended to the line.
 * If writing the line would exceed the maximum file size, the file is rotated
 * first.
 *
 * @param line The line to write
 */
void RingFile::writeLine(const std::string& line)
{
    if (_file)
    {
        const auto additional = line.size() + 1;

        _ensureOpenAndRotateIfNeeded(additional);

        _file << line << '\n';

        _bytesWritten += additional;
    }
    else
    {
        std::cerr << line << '\n';
    }
}

/**
 * @brief Writes text to the ring file
 *
 * @note If writing the text would exceed the maximum file size, the file is
 * rotated first.
 *
 * @param text The text to write
 */
void RingFile::write(const std::string& text)
{
    if (_file)
    {
        const auto additional = text.size();

        _ensureOpenAndRotateIfNeeded(additional);

        _file << text;

        _bytesWritten += additional;
    }
    else
    {
        std::cerr << text;
    }
}

/**
 * @brief Flushes the ring file
 */
void RingFile::flush()
{
    if (_file && _file.is_open())
        _file.flush();
}

/**
 * @brief Closes the ring file
 */
void RingFile::close()
{
    if (_file && _file.is_open())
        _file.close();
}

/**
 * @brief Returns the current log file path (index 0)
 *
 * @return std::filesystem::path The current log file path
 */
std::filesystem::path RingFile::getCurrentLogFilePath() const
{
    return _pathForIndex(0);
}

/**
 * @brief Normalizes the configuration
 *
 * @note Ensures maxFiles is at least 1, adds a dot to the extension if missing,
 * and sets the directory to the current directory if empty.
 */
void RingFile::_normalizeConfig()
{
    if (_config.maxFiles == 0)
        _config.maxFiles = 1;

    if (!_config.extension.empty() && _config.extension.front() != '.')
        _config.extension = "." + _config.extension;

    if (_config.directory.empty())
        _config.directory = ".";
}

/**
 * @brief Checks if writing additionalBytes would exceed the maximum file size
 *
 * @param additionalBytes The number of bytes to be written
 * @return true If writing would exceed the maximum file size
 * @return false Otherwise
 */
bool RingFile::_wouldExceed(std::uintmax_t additionalBytes) const
{
    if (_config.maxSizeMB == 0)
        return false;

    const auto logicalSize = _initialFileSize + _bytesWritten + additionalBytes;

    // TODO(97gamjak): clean this MB conversion later on
    // https://97gamjak.atlassian.net/browse/MOLTRACK-92
    return logicalSize > _config.maxSizeMB * Config::MBtoBytes;
}

/**
 * @brief Ensures the file is open and rotates if writing additionalBytes
 * would exceed the maximum file size
 *
 * @param additionalBytes The number of bytes to be written
 */
void RingFile::_ensureOpenAndRotateIfNeeded(std::uintmax_t additionalBytes)
{
    if (!_file.is_open())
        _openCurrent();

    if (_wouldExceed(additionalBytes))
    {
        _rotateNow();     // closes, shifts, truncates new current
        _openCurrent();   // re-open index 0 fresh
    }
}

/**
 * @brief Rotates the ring file
 *
 * @note Closes the current file, shifts existing files, and prepares for a new
 * current file.
 */
void RingFile::_rotateNow()
{
    // Must close before rename on Windows and to avoid buffered data issues.
    close();

    // Shift: (maxFiles-2)->(maxFiles-1), ..., 0->1
    if (_config.maxFiles > 1)
    {
        for (std::size_t i = _config.maxFiles - 1; i > 0; --i)
        {
            const auto src = _pathForIndex(i - 1);
            const auto dst = _pathForIndex(i);

            if (std::filesystem::exists(dst))
            {
                std::error_code errorCode;
                std::filesystem::remove(dst, errorCode);
            }

            if (std::filesystem::exists(src))
            {
                std::error_code errorCode;
                std::filesystem::rename(src, dst, errorCode);

                // If rename fails (e.g. cross-device), fall back to
                // copy+remove.
                if (errorCode)
                {
                    errorCode.clear();
                    std::filesystem::copy_file(
                        src,
                        dst,
                        std::filesystem::copy_options::overwrite_existing,
                        errorCode
                    );
                    if (!errorCode)
                        std::filesystem::remove(src, errorCode);
                }
            }
        }
    }
    else
    {
        // maxFiles == 1: just overwrite same file
        const auto path = _pathForIndex(0);
        if (std::filesystem::exists(path))
        {
            std::error_code errorCode;
            std::filesystem::remove(path, errorCode);
        }
    }
}

/**
 * @brief Opens the current ring file (index 0)
 *
 * @note Sets initial file size and resets bytes written counter.
 */
void RingFile::_openCurrent()
{
    const auto path = _pathForIndex(0);

    auto mode  = std::ios::out;
    mode      |= (_config.append ? std::ios::app : std::ios::trunc);

    // determine initial size before open if append
    if (_config.append && std::filesystem::exists(path))
        _initialFileSize = std::filesystem::file_size(path);
    else
        _initialFileSize = 0;

    _bytesWritten = 0;

    _file.open(path, mode);

    if (_config.symlinkPath.has_value())
    {
        if (_currentSymlinkPath != path)
        {
            std::error_code errorCode;
            std::filesystem::remove(_config.symlinkPath.value(), errorCode);
            if (errorCode)
            {
                std::cerr << "Failed to remove existing symlink: "
                          << errorCode.message() << "\n";
                return;
            }

            // Create new symlink pointing to the current log file
            auto target = std::filesystem::absolute(path);
            std::filesystem::create_symlink(
                target,
                _config.symlinkPath.value(),
                errorCode
            );
            if (errorCode)
            {
                std::cerr << "Failed to create symlink: " << errorCode.message()
                          << "\n";
                return;
            }

            _currentSymlinkPath = path;
        }
    }
}

/**
 * @brief Constructs the path for the given index
 *
 * @param index The index of the ring file
 * @return std::filesystem::path The constructed path
 */
std::filesystem::path RingFile::_pathForIndex(std::size_t index) const
{
    std::string buffer;

    if (_config.ignoreZeroIndex && index == 0)
    {
        // For index 0, when ignoreZeroIndex is enabled, use baseName +
        // extension
        buffer = std::format("{}{}", _config.baseName, _config.extension);
    }
    else
    {
        std::size_t nIndicesDigits = 1;
        if (_config.maxFiles > 1)
            nIndicesDigits = std::to_string(_config.maxFiles - 1).size();

        auto format = "{}_{:0" + std::to_string(nIndicesDigits) + "}{}";

        buffer = std::vformat(
            format,
            std::make_format_args(_config.baseName, index, _config.extension)
        );
    }

    return _config.directory / buffer;
}