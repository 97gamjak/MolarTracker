#include "utils/timestamp.hpp"

using std::chrono::current_zone;
using std::chrono::floor;
using std::chrono::milliseconds;
using std::chrono::seconds;
using std::chrono::zoned_time;

using Clock     = std::chrono::system_clock;
using Duration  = Clock::duration;
using TimePoint = std::chrono::local_time<Duration>;

/**
 * @brief Returns the current time point from the system clock
 *
 * @return TimePoint
 */
TimePoint Timestamp::now()
{
    const auto now = Clock::now();

    const auto* tz = current_zone();

    zoned_time local_time{tz, now};

    return local_time.get_local_time();
}

/**
 * @brief Returns the current time formatted as ISO-8601 in local time
 *
 * Example: 2026-01-20T12:34:05
 *
 * @return std::string
 */
std::string Timestamp::iso8601()
{
    return std::format("{:%FT%T}", floor<seconds>(now()));
}

/**
 * @brief Returns the current time formatted as ISO-8601 time only with
 * milliseconds precision
 *
 * Example: 12:34:05.123
 *
 * @return std::string
 */
std::string Timestamp::iso8601TimeMs()
{
    return std::format("{:%T}", floor<milliseconds>(now()));
}

/**
 * @brief Returns the current time formatted as human readable string
 *
 * Example: 2026-01-20 12:34:05
 *
 * @return std::string
 */
std::string Timestamp::humanReadable()
{
    return std::format("{:%Y-%m-%d %H:%M:%S}", floor<seconds>(now()));
}

/**
 * @brief Returns the current time formatted as filename-safe string
 *
 * Example: 20260120_123405
 *
 * @return std::string
 */
std::string Timestamp::fileSafe()
{
    return std::format("{:%Y%m%d_%H%M%S}", floor<seconds>(now()));
}