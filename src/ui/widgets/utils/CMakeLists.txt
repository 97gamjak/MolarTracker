add_library(molartracker_ui_utils STATIC
    discard_changes.cpp
)

target_include_directories(molartracker_ui_utils
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../..
)

target_link_libraries(molartracker_ui_utils
    PRIVATE
        Qt6::Widgets
)