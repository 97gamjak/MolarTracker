#ifndef __UTILS__RING_FILE_CONFIG_HPP__
#define __UTILS__RING_FILE_CONFIG_HPP__

#include <filesystem>
#include <string>

/**
 * @brief Configuration for the RingFile
 *
 */
struct RingFileConfig
{
    /// The directory where the log files will be stored
    std::filesystem::path directory{};

    /// The base name for the log files, e.g. if baseName is "log", the
    /// files will be named "log_0.txt", "log_1.txt", etc.
    std::string baseName{"log"};

    /// The extension for the log files, e.g. ".txt"
    std::string extension{".txt"};

    /// The maximum number of files to keep in the ring, when this number is
    /// exceeded, the oldest file will be deleted
    std::size_t maxFiles{10};

    /// The maximum size of each file in megabytes, when this size is
    /// exceeded, a new file will be created
    std::uintmax_t maxSizeMB{50};

    /// Whether to append to existing files or overwrite them when opening
    bool append{true};

    /// Whether to ignore the zero index file when rotating, if true, the
    /// first file will be "log_1.txt" instead of "log_0.txt", this can be
    /// useful to avoid confusion with non-rotating log files that might be
    /// named "log.txt"
    bool ignoreZeroIndex{true};
};

#endif   // __UTILS__RING_FILE_CONFIG_HPP__