add_library(molartracker_app_store_profile STATIC
    profile_store.cpp
    active_profile.cpp
)

target_include_directories(molartracker_app_store_profile
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../..
)

target_link_libraries(molartracker_app_store_profile
    PUBLIC
        molartracker_config
        molartracker_logging
        molartracker_app_domain
)