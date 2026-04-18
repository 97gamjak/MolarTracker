add_library(molartracker_settings STATIC
    src/settings/settings.cpp
    src/settings/ui_settings.cpp
    src/settings/log_viewer_settings.cpp
    src/settings/debug_slots_settings.cpp
    src/settings/profile_ui_settings.cpp
    src/settings/general_settings.cpp
    src/settings/logging_settings.cpp

    src/settings/params/param_container.cpp
    src/settings/params/param_error.cpp
)

target_include_directories(molartracker_settings
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(molartracker_settings
    PUBLIC
    json
    molartracker_json
    molartracker_connections
    molartracker_config
    PRIVATE
    molartracker_exceptions
    molartracker_utils
)
