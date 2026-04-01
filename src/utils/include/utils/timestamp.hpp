#ifndef __UTILS__INCLUDE__UTILS__TIMESTAMP_HPP__
#define __UTILS__INCLUDE__UTILS__TIMESTAMP_HPP__

#include <chrono>
#include <string>

using Clock     = std::chrono::system_clock;
using Duration  = Clock::duration;
using TimePoint = std::chrono::local_time<Duration>;

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
    TimePoint _timePoint;

   public:
    // Delete default constructor to prevent instantiation
    Timestamp();

    // ISO-8601 2026-01-20T11:34:05
    [[nodiscard]] std::string iso8601();

    // ISO-8601 time only with milliseconds precision: 12:34:05.123
    [[nodiscard]] std::string iso8601TimeMs();

    // Human readable: 2026-01-20 12:34:05
    [[nodiscard]] std::string humanReadable();

    // Filename-safe: 20260120_123405
    [[nodiscard]] std::string fileSafe();

   private:
    [[nodiscard]] static TimePoint _now();
};

#endif   // __UTILS__INCLUDE__UTILS__TIMESTAMP_HPP__