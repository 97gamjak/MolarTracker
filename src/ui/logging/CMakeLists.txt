add_library(molartracker_logging_ui STATIC
    debug_slots_dialog.cpp
    log_viewer_dialog.cpp
)

target_include_directories(molartracker_logging_ui
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../..
)

target_link_libraries(molartracker_logging_ui
    PRIVATE
        molartracker_logging
        molartracker_utils
        Qt6::Widgets
)