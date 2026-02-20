add_library(molartracker_settings STATIC
    settings.cpp
    ui_settings.cpp
    log_viewer_settings.cpp
    general_settings.cpp
    logging_settings.cpp
)

add_subdirectory(params)

target_include_directories(molartracker_settings
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

target_link_libraries(molartracker_settings
    PUBLIC
    molartracker_config
    molartracker_settings_params
)
