#include "utils/timestamp.hpp"

#include <chrono>
#include <format>

using std::chrono::current_zone;
using std::chrono::duration_cast;
using std::chrono::floor;
using std::chrono::milliseconds;
using std::chrono::seconds;
using std::chrono::zoned_time;

Timestamp::Timestamp() : _timePoint(Clock::now()) {}

/**
 * @brief Returns the current time point from the system clock
 *
 * @return TimePoint
 */
LocalTimePoint Timestamp::_toLocalTime() const
{
    const auto* timeZone = current_zone();

    zoned_time local_time{timeZone, _timePoint};

    return local_time.get_local_time();
}

/**
 * @brief Returns the current time formatted as ISO-8601 in local time
 *
 * Example: 2026-01-20T12:34:05
 *
 * @return std::string
 */
std::string Timestamp::iso8601() const
{
    return std::format("{:%FT%T}", floor<seconds>(_toLocalTime()));
}

/**
 * @brief Returns the current time formatted as ISO-8601 time only with
 * milliseconds precision
 *
 * Example: 12:34:05.123
 *
 * @return std::string
 */
std::string Timestamp::iso8601TimeMs() const
{
    return std::format("{:%T}", floor<milliseconds>(_toLocalTime()));
}

/**
 * @brief Returns the current time formatted as human readable string
 *
 * Example: 2026-01-20 12:34:05
 *
 * @return std::string
 */
std::string Timestamp::humanReadable() const
{
    return std::format("{:%Y-%m-%d %H:%M:%S}", floor<seconds>(_toLocalTime()));
}

/**
 * @brief Returns the current time formatted as filename-safe string
 *
 * Example: 20260120_123405
 *
 * @return std::string
 */
std::string Timestamp::fileSafe() const
{
    return std::format("{:%Y%m%d_%H%M%S}", floor<seconds>(_toLocalTime()));
}

[[nodiscard]] int64_t Timestamp::toInt64() const
{
    return duration_cast<milliseconds>(_toLocalTime().time_since_epoch())
        .count();
}
