add_library(molartracker_logging STATIC
    src/logging/log_manager.cpp
    src/logging/log_entry_scope.cpp
    src/logging/log_object.cpp
    src/logging/log_category.cpp
    src/logging/log_categories.cpp
    src/logging/log_exceptions.cpp
)

target_include_directories(molartracker_logging
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(molartracker_logging
    PRIVATE
    molartracker_log_categories_generated
    molartracker_settings
    molartracker_exceptions
    molartracker_settings
    mstd
    PUBLIC
    molartracker_utils # value storing ring file
    molartracker_config
)

set_property(TARGET molartracker_logging APPEND PROPERTY
    AUTOGEN_TARGET_DEPENDS generate_log_categories
)