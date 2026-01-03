add_library(molartracker_app_domain STATIC
    profile.cpp
)

target_include_directories(molartracker_app_domain
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

target_link_libraries(molartracker_app_domain
    PUBLIC
    molartracker_config
)