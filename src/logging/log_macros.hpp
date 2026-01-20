#ifndef __LOGGING__LOG_MACROS_HPP__
#define __LOGGING__LOG_MACROS_HPP__

#include "log_manager.hpp"
#include "mstd/error.hpp"

// clang-format off
#define LOG_INTERNAL(level, category, message)  \
    LogManager::getInstance().log(              \
        level,                                  \
        category,                               \
        __FILE__,                               \
        __LINE__,                               \
        __func__,                               \
        message                                 \
    )
// clang-format on

// clang-format off
#ifndef __LOG_CATEGORY__
    #define LOG(level, message)                                                 \
        MSTD_COMPILE_FAIL(                                                      \
            "LOG_CATEGORY must be defined before including log_manager.hpp"     \
        );
#else
    #define LOG(level, message) LOG_INTERNAL(level, __LOG_CATEGORY__, message)
#endif
// clang-format on

#endif   // __LOGGING__LOG_MACROS_HPP__