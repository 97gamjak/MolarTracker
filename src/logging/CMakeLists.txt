add_library(molartracker_logging STATIC
    log_manager.cpp
    log_entry_scope.cpp
    log_object.cpp
    log_category.cpp
    log_categories.cpp
    log_exceptions.cpp
)

target_include_directories(molartracker_logging
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/..
)

target_link_libraries(molartracker_logging
    PRIVATE
    molartracker_log_categories_generated
    molartracker_settings
    molartracker_config
    PUBLIC
    mstd
)

set_property(TARGET molartracker_logging APPEND PROPERTY
    AUTOGEN_TARGET_DEPENDS generate_log_categories
)