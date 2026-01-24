add_library(molartracker_logging STATIC
    log_manager.cpp
    log_entry_scope.cpp
)

if(MSVC)
    target_compile_options(molartracker_logging
        PRIVATE
        /Zc:preprocessor
    )
endif()

target_include_directories(molartracker_logging
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${PROJECT_SOURCE_DIR}/external/mstd/include
)