#ifndef __CONFIG__LOGGING_BASE_HPP__
#define __CONFIG__LOGGING_BASE_HPP__

#include <cstddef>
#include <mstd/enum.hpp>
#include <unordered_map>

#define LOG_CATEGORY(X)           \
    X(app_service_profileService) \
    X(app_storeContainer)         \
    X(app_store_ProfileStore)     \
    X(ui_AddProfileCommand)       \
    X(ui_mainWindow)              \
    X(ui_logging)                 \
    X(ui_debugMenuController)     \
    X(ui_ensureProfileController) \
    X(ui_profileSelectionDialog)  \
    X(database)                   \
    X(logging_manager)            \
    X(application)

MSTD_ENUM(LogCategory, std::size_t, LOG_CATEGORY);

#define LOG_LEVEL(X) \
    X(Off, 0)        \
    X(Error)         \
    X(Warning)       \
    X(Info)          \
    X(Debug)         \
    X(Trace)

MSTD_ENUM(LogLevel, std::size_t, LOG_LEVEL);

using LogCategoryMap = std::unordered_map<LogCategory, LogLevel>;

#endif   // __CONFIG__LOGGING_BASE_HPP__