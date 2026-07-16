#include "utils/timestamp.hpp"

#include <iostream>

#ifdef __QT_ENABLED__
#include <qdatetime.h>
#endif

#include <chrono>
#include <format>

using Clock     = std::chrono::system_clock;
using TimePoint = Clock::time_point;
using std::chrono::current_zone;
using std::chrono::duration_cast;
using std::chrono::floor;
using std::chrono::milliseconds;
using std::chrono::nanoseconds;
using std::chrono::seconds;
using std::chrono::zoned_time;

namespace
{
    /**
     * @brief Converts an int64 time point to a TimePoint object.
     *
     * @param timePoint
     * @return TimePoint
     */
    TimePoint toTimePoint(int64_t timePoint)
    {
        return TimePoint{milliseconds{timePoint}};
    }
}   // namespace

/**
 * @brief Constructs a Timestamp object representing the current time.
 */
Timestamp::Timestamp()
    : _timePoint(
          duration_cast<milliseconds>(Clock::now().time_since_epoch()).count()
      )
{
}

/**
 * @brief Returns the current time point from the system clock
 *
 * @return int64_t
 */
int64_t Timestamp::_toLocalTime() const
{
    const auto* timeZone = current_zone();

    zoned_time local_time{timeZone, toTimePoint(_timePoint)};

    return duration_cast<milliseconds>(
               local_time.get_local_time().time_since_epoch()
    )
        .count();
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
    return std::format("{:%FT%T}", floor<seconds>(toTimePoint(_toLocalTime())));
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
    return std::format(
        "{:%T}",
        floor<milliseconds>(toTimePoint(_toLocalTime()))
    );
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
    return std::format(
        _humanReadableFmt,
        floor<seconds>(toTimePoint(_toLocalTime()))
    );
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
    return std::format(
        _fileSafeFmt,
        floor<seconds>(toTimePoint(_toLocalTime()))
    );
}

/**
 * @brief Returns the current time formatted as a date string
 *
 * Example: 260120
 *
 * @return std::string
 */
std::string Timestamp::toDateString() const
{
    return std::format(
        "{:%y%m%d}",
        floor<seconds>(toTimePoint(_toLocalTime()))
    );
}

/**
 * @brief Converts the Timestamp to an int64 representation.
 *
 * @return int64_t
 */
int64_t Timestamp::toInt64() const { return _timePoint; }

/**
 * @brief Construct a new Timestamp:: Timestamp object
 *
 * @param timePoint
 */
Timestamp::Timestamp(int64_t timePoint) : _timePoint(timePoint) {}

/**
 * @brief Constructs a Timestamp object from an int64 representation.
 *
 * @param value The int64 representation of the Timestamp.
 * @return Timestamp
 */
Timestamp Timestamp::fromInt64(int64_t value) { return Timestamp(value); }

/**
 * @brief Returns a Timestamp object representing a null timestamp.
 *
 * @return Timestamp
 */
Timestamp Timestamp::Null() { return Timestamp(0); }

#ifdef __QT_ENABLED__
/**
 * @brief Constructs a Timestamp object from a QDateTime object.
 *
 * @param dateTime The QDateTime object to use for the Timestamp.
 */
Timestamp::Timestamp(const QDateTime& dateTime)
    : _timePoint(dateTime.toMSecsSinceEpoch())
{
}

/**
 * @brief Converts the Timestamp to a QDateTime object.
 *
 * @return QDateTime
 */
QDateTime Timestamp::toQDateTime() const
{
    // TODO(97gamjak): implement this with timezone support
    // https://97gamjak.atlassian.net/browse/MOLTRACK-95
    return QDateTime::fromMSecsSinceEpoch(toInt64());
}

/**
 * @brief Compares this Timestamp with another Timestamp for ordering.
 *
 * @param other The other Timestamp to compare against.
 * @return std::strong_ordering indicating the relative order of the two
 * Timestamps.
 */
std::strong_ordering Timestamp::operator<=>(const Timestamp& other) const
{
    return _timePoint <=> other._timePoint;
}

#endif

/**
 * @brief parses a filename-safe timestamp string and returns an optional
 * Timestamp, if the parsing fails, returns std::nullopt
 *
 * @param value
 * @param prefix
 * @param postfix
 * @return std::optional<Timestamp>
 */
std::optional<Timestamp> Timestamp::fromFileSafe(
    const std::string& value,
    const std::string& prefix,
    const std::string& postfix
)
{
    auto valueWithoutPrefix =
        value.starts_with(prefix) ? value.substr(prefix.size()) : value;

    valueWithoutPrefix = valueWithoutPrefix.starts_with("_")
                             ? valueWithoutPrefix.substr(1)
                             : valueWithoutPrefix;

    if (valueWithoutPrefix.ends_with(postfix))
    {
        valueWithoutPrefix = valueWithoutPrefix.substr(
            0,
            valueWithoutPrefix.size() - postfix.size()
        );
    }

    valueWithoutPrefix =
        valueWithoutPrefix.ends_with(".")
            ? valueWithoutPrefix.substr(0, valueWithoutPrefix.size() - 1)
            : valueWithoutPrefix;

    std::istringstream       stream{valueWithoutPrefix};
    std::chrono::sys_seconds timePoint;
    stream >> std::chrono::parse(_fileSafeParseFmt, timePoint);

    if (stream.fail())
        return std::nullopt;

    return Timestamp(
        duration_cast<milliseconds>(timePoint.time_since_epoch()).count()
    );
}

/**
 * @brief Returns the year component of the timestamp.
 *
 * @return int The year component.
 */
int Timestamp::year() const
{
    const auto days = floor<std::chrono::days>(toTimePoint(_toLocalTime()));
    const std::chrono::year_month_day ymd{days};
    return static_cast<int>(ymd.year());
}

/**
 * @brief Returns the month component of the timestamp.
 *
 * @return unsigned int The month component (1-12).
 */
unsigned int Timestamp::month() const
{
    const auto days = floor<std::chrono::days>(toTimePoint(_toLocalTime()));
    const std::chrono::year_month_day ymd{days};
    return static_cast<unsigned int>(ymd.month());
}

/**
 * @brief Returns the day component of the timestamp.
 *
 * @return unsigned int The day component (1-31).
 */
unsigned int Timestamp::day() const
{
    const auto days = floor<std::chrono::days>(toTimePoint(_toLocalTime()));
    const std::chrono::year_month_day ymd{days};
    return static_cast<unsigned int>(ymd.day());
}

/**
 * @brief Returns the week number of the timestamp.
 *
 * @return unsigned int The week number (1-53).
 */
unsigned int Timestamp::week() const
{
    const auto days = floor<std::chrono::days>(toTimePoint(_toLocalTime()));
    return static_cast<unsigned int>(days.time_since_epoch().count() / 7);
}