add_library(molartracker_ui_logging STATIC
    debug_slots_dialog.cpp
    log_viewer_dialog.cpp
)

target_include_directories(molartracker_ui_logging
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../../..
)

target_link_libraries(molartracker_ui_logging
    PRIVATE
        molartracker_logging
        molartracker_utils
        molartracker_ui_utils
        Qt6::Widgets
)