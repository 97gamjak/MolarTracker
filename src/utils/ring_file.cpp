#include "utils/ring_file.hpp"

#include <filesystem>

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
    for (std::size_t i = 0; i < _config.max_files; ++i)
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
    if (_config.max_files == 0)
        _config.max_files = 1;

    if (_config.extension.empty() || _config.extension.front() != '.')
        _config.extension = "." + _config.extension;

    if (_config.directory.empty())
        _config.directory = ".";
}

void RingFile::_rotateIfNeeded()
{
    _current_index = _findNextIndex();

    const auto p = _pathForIndex(_current_index);
    if (std::filesystem::exists(p))
    {
        std::error_code ec;
        std::filesystem::remove(p, ec);
    }
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
    // Picks the oldest file index to overwrite based on
    // last_write_time. If not all files exist yet, picks the first
    // missing index.

    std::vector<std::pair<std::size_t, std::filesystem::file_time_type>>
        existing;

    for (std::size_t i = 0; i < _config.max_files; ++i)
    {
        const auto p = _pathForIndex(i);
        if (!std::filesystem::exists(p))
            return i;

        existing.push_back({i, std::filesystem::last_write_time(p)});
    }

    const auto it = std::ranges::min_element(
        existing,
        [](const auto& a, const auto& b) { return a.second < b.second; }
    );

    return it->first;
}

std::filesystem::path RingFile::_pathForIndex(std::size_t index) const
{
    // TODO(97gamjak): implement number of indices according to max files
    // ... issue link
    // TODO: clean this up!!!
    char buffer[64]{};
    std::snprintf(
        buffer,
        sizeof(buffer),
        "%s_%04zu%s",
        _config.base_name.c_str(),
        index,
        _config.extension.c_str()
    );

    return _config.directory / buffer;
}