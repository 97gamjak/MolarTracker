#include "utils/timestamp.hpp"

using std::chrono::duration_cast;
using std::chrono::floor;
using std::chrono::milliseconds;
using std::chrono::seconds;

/**
 * @brief Returns the current time point from the system clock
 *
 * @return Timestamp::TimePoint
 */
Timestamp::TimePoint Timestamp::now() { return Clock::now(); }

/**
 * @brief Returns the current Unix timestamp in seconds
 *
 * @return std::int64_t
 */
std::int64_t Timestamp::unixSeconds()
{
    return duration_cast<seconds>(now().time_since_epoch()).count();
}

/**
 * @brief Returns the current Unix timestamp in milliseconds
 *
 * @return std::int64_t
 */
std::int64_t Timestamp::unixMilliseconds()
{
    return duration_cast<milliseconds>(now().time_since_epoch()).count();
}

/**
 * @brief Returns the current time formatted as ISO-8601 in UTC
 *
 * Example: 2026-01-20T11:34:05Z
 *
 * @return std::string
 */
std::string Timestamp::iso8601Utc()
{
    return std::format("{:%FT%TZ}", floor<seconds>(now()));
}

/**
 * @brief Returns the current time formatted as ISO-8601 in local time
 *
 * Example: 2026-01-20T12:34:05
 *
 * @return std::string
 */
std::string Timestamp::iso8601Local()
{
    return std::format("{:%FT%T}", floor<seconds>(now()));
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