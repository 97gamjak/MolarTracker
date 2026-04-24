add_library(molartracker_utils STATIC
    src/utils/timestamp.cpp
    src/utils/ring_file.cpp
    src/utils/paths.cpp
    src/utils/qt_helpers.cpp
    src/utils/version.cpp
)

target_include_directories(molartracker_utils
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(molartracker_utils
    PUBLIC
    mstd
    PRIVATE
    Qt6::Core
    Qt6::Widgets
    json
)

set_property(TARGET molartracker_utils APPEND PROPERTY
    AUTOGEN_TARGET_DEPENDS generate_log_categories
)

# add qt enabled to compile time flags for timestamp
target_compile_definitions(molartracker_utils
    PRIVATE
    __QT_ENABLED__
)