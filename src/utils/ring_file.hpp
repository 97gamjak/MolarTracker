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
        std::size_t           maxSizeMB{50};
    };

   private:
    Config        _config{};
    std::size_t   _current_index{0};
    std::ofstream _file{};

   public:
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

    std::filesystem::path              currentPath() const;
    std::vector<std::filesystem::path> listFilesSorted() const;

   private:
    void _normalizeConfig();
    void _rotateIfNeeded();
    void _openCurrent();

    std::filesystem::path _pathForIndex(std::size_t index) const;
    std::size_t           _findNextIndex() const;
};

#endif   // __UTILS__RING_FILE_HPP__