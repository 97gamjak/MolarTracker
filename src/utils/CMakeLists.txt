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
)

target_link_libraries(molartracker_utils
    PUBLIC
        Qt6::Core
        Qt6::Widgets
        json
        molartracker_logging
)

set_property(TARGET molartracker_utils APPEND PROPERTY
    AUTOGEN_TARGET_DEPENDS generate_log_categories
)