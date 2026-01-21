#ifndef __UTILS__TIMESTAMP_HPP__
#define __UTILS__TIMESTAMP_HPP__

#include <chrono>
#include <format>
#include <string>

// TODO(97gamjak): migrate to mstd later on
// https://97gamjak.atlassian.net/browse/MSTD-89

/**
 * @brief Utility class for handling timestamps
 */
class Timestamp final
{
   public:
    using Clock     = std::chrono::system_clock;
    using TimePoint = Clock::time_point;

    // Delete default constructor to prevent instantiation
    Timestamp() = delete;

    [[nodiscard]] static TimePoint now();

    [[nodiscard]] static std::int64_t unixSeconds();

    [[nodiscard]] static std::int64_t unixMilliseconds();

    // ISO-8601, UTC: 2026-01-20T11:34:05Z
    [[nodiscard]] static std::string iso8601Utc();

    // ISO-8601, local time: 2026-01-20T12:34:05
    [[nodiscard]] static std::string iso8601Local();

    // Human readable: 2026-01-20 12:34:05
    [[nodiscard]] static std::string humanReadable();

    // Filename-safe: 20260120_123405
    [[nodiscard]] static std::string fileSafe();
};

#endif   // __UTILS__TIMESTAMP_HPP__