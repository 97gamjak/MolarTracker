#ifndef __LOGGING__LOGLEVEL_HPP__
#define __LOGGING__LOGLEVEL_HPP__

/**
 * @brief Log levels for logging messages
 */
enum class LogLevel
{
    Off = 0,
    Critical,
    Error,
    Warning,
    Info,
    Debug,
    Trace
};

#endif   // __LOGGING__LOGLEVEL_HPP__