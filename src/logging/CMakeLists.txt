add_library(molartracker_logging STATIC
    log_manager.cpp
    log_entry_scope.cpp
)

target_include_directories(molartracker_logging
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${PROJECT_SOURCE_DIR}/external/mstd/include
    ${PROJECT_SOURCE_DIR}/external/json/single_include
)

target_link_libraries(molartracker_logging
    PUBLIC
        molartracker_utils
)