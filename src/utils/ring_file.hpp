#ifndef __UTILS__RING_FILE_HPP__
#define __UTILS__RING_FILE_HPP__

#include <filesystem>
#include <fstream>
#include <string>

#include "ring_file_config.hpp"

// TODO: migrate this RingFile to mstd
// https://97gamjak.atlassian.net/browse/MSTD-90

/**
 * @brief A class for writing ring files
 *
 * A ring file is a set of files that are used in a circular manner. When the
 * current file reaches a specified maximum size, it is closed and a new file is
 * opened. The oldest file is deleted when the maximum number of files is
 * reached.
 *
 */
class RingFile
{
   private:
    /// type alias for the configuration struct
    using Config = RingFileConfig;

   private:
    /// The configuration for the RingFile
    Config _config{};

    /// The currently open file stream for writing log entries
    std::ofstream _file{};

    /// The index of the current file in the ring
    std::uintmax_t _initialFileSize{0};

    /// The total number of bytes written to the current file
    std::uintmax_t _bytesWritten{0};

   public:
    // TODO: as soon as migration to mstd is done, create a nice Sink interface,
    // so that std::cerr can be passed as a sink
    // https://97gamjak.atlassian.net/browse/MOLTRACK-93
    explicit RingFile() = default;   // for std::cerr outputs
    explicit RingFile(const Config& config);

    ~RingFile();

    RingFile(const RingFile&)            = delete;
    RingFile& operator=(const RingFile&) = delete;

    RingFile(RingFile&& other);
    RingFile& operator=(RingFile&& other);

    void writeLine(const std::string& line);
    void write(const std::string& text);

    void flush();
    void close();

    std::filesystem::path getCurrentLogFilePath() const;

   private:
    void _normalizeConfig();
    void _openCurrent();

    bool _wouldExceed(const std::uintmax_t additionalBytes) const;
    void _ensureOpenAndRotateIfNeeded(const std::uintmax_t additionalBytes);
    void _rotateNow();

    std::filesystem::path _pathForIndex(const std::size_t index) const;
};

#endif   // __UTILS__RING_FILE_HPP__