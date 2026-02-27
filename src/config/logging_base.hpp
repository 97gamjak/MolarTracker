#ifndef __CONFIG__LOGGING_BASE_HPP__
#define __CONFIG__LOGGING_BASE_HPP__

#include <cstdint>
#include <mstd/enum.hpp>

// NOLINTBEGIN(cppcoreguidelines-macro-usage)

#define LOG_LEVEL(X) \
    X(Off, 0)        \
    X(Error)         \
    X(Warning)       \
    X(Info)          \
    X(Debug)         \
    X(Trace)

// cppcheck-suppress syntaxError
MSTD_ENUM(LogLevel, std::int8_t, LOG_LEVEL);

// NOLINTEND

#endif   // __CONFIG__LOGGING_BASE_HPP__