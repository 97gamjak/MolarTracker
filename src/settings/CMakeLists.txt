add_library(molartracker_settings STATIC
    settings.cpp
    ui_settings.cpp
    log_viewer_settings.cpp
    param_error.cpp
)

target_include_directories(molartracker_settings
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${CMAKE_CURRENT_SOURCE_DIR}/../../external/mstd/include
)

target_link_libraries(molartracker_settings
    PUBLIC
    molartracker_config
)
