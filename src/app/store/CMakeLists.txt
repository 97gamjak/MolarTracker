add_library(molartracker_app_store STATIC
    account_store.cpp

    profile/profile_store.cpp
    profile/active_profile.cpp
    profile/exception.cpp
)

target_include_directories(molartracker_app_store
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

target_link_libraries(molartracker_app_store
    PUBLIC
        molartracker_app_domain
        molartracker_config
        molartracker_logging
)