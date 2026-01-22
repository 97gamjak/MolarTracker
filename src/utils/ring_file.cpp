#include "utils/ring_file.hpp"

#include <filesystem>
#include <format>

RingFile::RingFile(const Config& config) : _config(config)
{
    _normalizeConfig();
    std::filesystem::create_directories(_config.directory);
    _rotateIfNeeded();
    _openCurrent();
}

RingFile::~RingFile()
{
    flush();
    close();
}

RingFile::RingFile(RingFile&& other) { *this = std::move(other); }

RingFile& RingFile::operator=(RingFile&& other)
{
    if (this == &other)
        return *this;

    close();
    _config              = std::move(other._config);
    _current_index       = other._current_index;
    _file                = std::move(other._file);
    other._current_index = 0;
    return *this;
}

void RingFile::writeLine(const std::string& line)
{
    if (!_file.is_open())
    {
        _rotateIfNeeded();
        _openCurrent();
    }

    _file << line << '\n';
}

void RingFile::write(const std::string& text)
{
    if (!_file.is_open())
    {
        _rotateIfNeeded();
        _openCurrent();
    }

    _file << text;
}

void RingFile::flush()
{
    if (_file.is_open())
        _file.flush();
}

void RingFile::close()
{
    if (_file.is_open())
        _file.close();
}

std::filesystem::path RingFile::currentPath() const
{
    return _pathForIndex(_current_index);
}

std::vector<std::filesystem::path> RingFile::listFilesSorted() const
{
    std::vector<std::filesystem::path> result;
    for (std::size_t i = 0; i < _config.maxFiles; ++i)
    {
        const auto p = _pathForIndex(i);
        if (std::filesystem::exists(p))
            result.push_back(p);
    }

    std::sort(result.begin(), result.end());
    return result;
}

void RingFile::_normalizeConfig()
{
    if (_config.maxFiles == 0)
        _config.maxFiles = 1;

    if (!_config.extension.empty() && _config.extension.front() != '.')
        _config.extension = "." + _config.extension;

    if (_config.directory.empty())
        _config.directory = ".";
}

void RingFile::_rotateIfNeeded()
{
    // check if we need a new file
    const auto p = _pathForIndex(0);

    if (std::filesystem::file_size(p) > 1024 * 1024)
    {
        // we need to rotate!!! so pushing all files back for at one index!
        for (std::size_t i = _config.maxFiles - 1; i > 0; --i)
        {
            const auto      src = _pathForIndex(i - 1);
            const auto      dst = _pathForIndex(i);
            std::error_code ec;
            std::filesystem::rename(src, dst, ec);
        }
    }

    // we need to open the file!!!
}

void RingFile::_openCurrent()
{
    const auto p = _pathForIndex(_current_index);

    std::ios::openmode mode = std::ios::out;
    mode |= (_config.append ? std::ios::app : std::ios::trunc);

    _file.open(p, mode);
}

std::size_t RingFile::_findNextIndex() const
{
    for (std::size_t i = 0; i < _config.maxFiles; ++i)
    {
        const auto p = _pathForIndex(i);
        if (!std::filesystem::exists(p))
            return i;
    }

    return 0;
}

std::filesystem::path RingFile::_pathForIndex(std::size_t index) const
{
    std::size_t nIndicesDigits = (_config.maxFiles - 1) / 10;

    std::string format = "{}_{:0" + std::to_string(nIndicesDigits) + "}{}";

    if (_config.ignoreZeroIndex && index == 0)
        format = "{}{}";

    std::string buffer = std::vformat(
        format,
        std::make_format_args(_config.baseName, index, _config.extension)
    );

    return _config.directory / buffer;
}