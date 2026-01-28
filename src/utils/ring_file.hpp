#ifndef __UTILS__RING_FILE_HPP__
#define __UTILS__RING_FILE_HPP__

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <string>
#include <system_error>
#include <vector>

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
   public:
    struct Config
    {
        std::filesystem::path directory{};
        std::string           baseName{"log"};
        std::string           extension{".txt"};
        std::size_t           maxFiles{10};
        bool                  append{true};
        bool                  ignoreZeroIndex{true};
        std::uintmax_t        maxSizeMB{50};
    };

   private:
    Config         _config{};
    std::ofstream  _file{};
    std::uintmax_t _initialFileSize{0};
    std::uintmax_t _bytesWritten{0};

   public:
    // TODO: as soon as migration to mstd is done, create a nice Sink interface,
    // so that std::cerr can be passed as a sink
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