add_library(molartracker_app_store STATIC
    profile_store.cpp
)

target_include_directories(molartracker_app_store
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

target_link_libraries(molartracker_app_store
    PUBLIC
        molartracker_config
        molartracker_logging
        molartracker_app_domain
)