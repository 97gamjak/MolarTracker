add_library(molartracker_common STATIC
    src/common/cash.cpp
    src/common/currency.cpp
    src/common/currency_exception.cpp
    src/common/ring_file.cpp
    src/common/paths.cpp
    src/common/percentage.cpp
    src/common/qt_helpers.cpp
    src/common/quantity.cpp
    src/common/shortcut.cpp
    src/common/shortcut_set.cpp
    src/common/timestamp.cpp
    src/common/version.cpp
)

target_include_directories(molartracker_common
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(molartracker_common
    PUBLIC
    mstd
    PRIVATE
    Qt6::Core
    Qt6::Widgets
    json
    molartracker_exceptions
    molartracker_config
)

set_property(TARGET molartracker_common APPEND PROPERTY
    AUTOGEN_TARGET_DEPENDS generate_log_categories
)

# add qt enabled to compile time flags for timestamp
target_compile_definitions(molartracker_common
    PRIVATE
    __QT_ENABLED__
)