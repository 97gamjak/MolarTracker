add_library(molartracker_logging STATIC
    log_manager.cpp
    log_entry_scope.cpp
    log_object.cpp
    log_category.cpp
)

target_include_directories(molartracker_logging
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/..
)

target_link_libraries(molartracker_logging
    PUBLIC
        molartracker_utils
        molartracker_config
        molartracker_settings
        molartracker_log_categories_generated
)