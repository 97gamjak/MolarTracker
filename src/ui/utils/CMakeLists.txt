add_library(molartracker_utils_ui STATIC
    discard_changes.cpp
)

target_include_directories(molartracker_utils_ui
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../..
)

target_link_libraries(molartracker_utils_ui
    PRIVATE
        Qt6::Widgets
)