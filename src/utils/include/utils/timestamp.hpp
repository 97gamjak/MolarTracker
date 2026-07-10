#ifndef __UTILS__INCLUDE__UTILS__TIMESTAMP_HPP__
#define __UTILS__INCLUDE__UTILS__TIMESTAMP_HPP__

#include <optional>
#include <string>

// TODO(97gamjak): migrate to mstd later on
// https://97gamjak.atlassian.net/browse/MSTD-89
// https://97gamjak.atlassian.net/browse/MOLTRACK-94

// TODO(97gamjak): implement this with timezone support
// https://97gamjak.atlassian.net/browse/MOLTRACK-95

class QDateTime;   // Forward declaration

/**
 * @brief Utility class for handling timestamps
 */
class Timestamp
{
   private:
    /// The underlying time point representing the timestamp.
    int64_t _timePoint;

    /// Indicates whether the return timestamp should be converted to local time
    bool _localTime = true;

    static constexpr auto humanReadableFmt = "{:%Y-%m-%d %H:%M:%S}";

   public:
    Timestamp();
    explicit Timestamp(const QDateTime& dateTime);

    [[nodiscard]]
    static Timestamp Null();
    [[nodiscard]]
    static Timestamp fromInt64(int64_t value);
    [[nodiscard]]
    static std::optional<Timestamp> fromHumanReadable(const std::string& value);

    // ISO-8601 2026-01-20T11:34:05
    [[nodiscard]] std::string iso8601() const;

    // ISO-8601 time only with milliseconds precision: 12:34:05.123
    [[nodiscard]] std::string iso8601TimeMs() const;

    // Human readable: 2026-01-20 12:34:05
    [[nodiscard]] std::string humanReadable() const;

    // Filename-safe: 20260120_123405
    [[nodiscard]] std::string fileSafe() const;

    [[nodiscard]] int64_t toInt64() const;

    [[nodiscard]] QDateTime toQDateTime() const;

    [[nodiscard]] int          year() const;
    [[nodiscard]] unsigned int month() const;
    [[nodiscard]] unsigned int day() const;

   private:
    explicit Timestamp(int64_t timePoint);

    [[nodiscard]] int64_t _toLocalTime() const;
};

#endif   // __UTILS__INCLUDE__UTILS__TIMESTAMP_HPP__