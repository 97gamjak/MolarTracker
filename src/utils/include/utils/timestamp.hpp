#ifndef __UTILS__INCLUDE__UTILS__TIMESTAMP_HPP__
#define __UTILS__INCLUDE__UTILS__TIMESTAMP_HPP__

#include <chrono>
#include <string>

using Clock          = std::chrono::system_clock;
using Duration       = Clock::duration;
using TimePoint      = Clock::time_point;
using LocalTimePoint = std::chrono::local_time<Duration>;

// TODO(97gamjak): migrate to mstd later on
// https://97gamjak.atlassian.net/browse/MSTD-89
// https://97gamjak.atlassian.net/browse/MOLTRACK-94

// TODO(97gamjak): implement this with timezone support
// https://97gamjak.atlassian.net/browse/MOLTRACK-95

/**
 * @brief Utility class for handling timestamps
 */
class Timestamp
{
   private:
    TimePoint _timePoint;

    bool _localTime = true;

   public:
    // Delete default constructor to prevent instantiation
    Timestamp();
    explicit Timestamp(const TimePoint& timePoint);

    // ISO-8601 2026-01-20T11:34:05
    [[nodiscard]] std::string iso8601() const;

    // ISO-8601 time only with milliseconds precision: 12:34:05.123
    [[nodiscard]] std::string iso8601TimeMs() const;

    // Human readable: 2026-01-20 12:34:05
    [[nodiscard]] std::string humanReadable() const;

    // Filename-safe: 20260120_123405
    [[nodiscard]] std::string fileSafe() const;

    [[nodiscard]] int64_t toInt64() const;

    [[nodiscard]] static Timestamp fromInt64(int64_t value);

   private:
    [[nodiscard]] LocalTimePoint _toLocalTime() const;
};

#endif   // __UTILS__INCLUDE__UTILS__TIMESTAMP_HPP__