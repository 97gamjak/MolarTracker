add_library(molartracker_utils STATIC
    timestamp.cpp
    ring_file.cpp
    paths.cpp
    qt_helpers.cpp
    version.cpp
)

target_include_directories(molartracker_utils
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/..
        ${PROJECT_SOURCE_DIR}/external/json/single_include
)

target_link_libraries(molartracker_utils
    PUBLIC
        Qt6::Core
        Qt6::Widgets
)