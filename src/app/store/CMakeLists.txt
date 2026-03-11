add_library(molartracker_app_store STATIC
    i_store.cpp
    account_store.cpp
)

add_subdirectory(profile_store)

target_include_directories(molartracker_app_store
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

target_link_libraries(molartracker_app_store
    PUBLIC
        molartracker_app_domain
        molartracker_app_store_profile
        molartracker_config
        molartracker_logging
)