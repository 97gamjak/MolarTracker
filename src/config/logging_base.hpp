#ifndef __CONFIG__LOGGING_BASE_HPP__
#define __CONFIG__LOGGING_BASE_HPP__

#include <cstdint>
#include <mstd/enum.hpp>

#define LOG_LEVEL(X) \
    X(Off, 0)        \
    X(Error)         \
    X(Warning)       \
    X(Info)          \
    X(Debug)         \
    X(Trace)

// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
// cppcheck-suppress syntaxError
MSTD_ENUM(LogLevel, std::int8_t, LOG_LEVEL);

#endif   // __CONFIG__LOGGING_BASE_HPP__