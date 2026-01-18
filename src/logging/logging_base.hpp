#ifndef __LOGGING__LOGGING_BASE_HPP__
#define __LOGGING__LOGGING_BASE_HPP__

#include <cstddef>
#include <mstd/enum.hpp>

#define LOG_CATEGORY(X)            \
    X(app_service_profile_service) \
    X(app_ui_main_window)

MSTD_ENUM(LogCategory, size_t, LOG_CATEGORY)

#endif   // __LOGGING__LOGGING_BASE_HPP__