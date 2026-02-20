#ifndef __UTILS__TIMESTAMP_HPP__
#define __UTILS__TIMESTAMP_HPP__

#include <chrono>
#include <format>
#include <string>

// TODO(97gamjak): migrate to mstd later on
// https://97gamjak.atlassian.net/browse/MSTD-89
// https://97gamjak.atlassian.net/browse/MOLTRACK-94

// TODO: implement this with timezone support
// https://97gamjak.atlassian.net/browse/MOLTRACK-95

/**
 * @brief Utility class for handling timestamps
 */
class Timestamp final
{
   public:
    // Delete default constructor to prevent instantiation
    Timestamp() = delete;

    [[nodiscard]] static std::chrono::local_time<std::chrono::system_clock::duration> now(
    );
    // ISO-8601 2026-01-20T11:34:05
    [[nodiscard]] static std::string iso8601();

    // ISO-8601 time only with milliseconds precision: 12:34:05.123
    [[nodiscard]] static std::string iso8601TimeMs();

    // Human readable: 2026-01-20 12:34:05
    [[nodiscard]] static std::string humanReadable();

    // Filename-safe: 20260120_123405
    [[nodiscard]] static std::string fileSafe();
};

#endif   // __UTILS__TIMESTAMP_HPP__