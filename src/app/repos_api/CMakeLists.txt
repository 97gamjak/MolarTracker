add_library(molartracker_app_repos_api INTERFACE)

target_include_directories(molartracker_app_repos_api
    INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

target_link_libraries(molartracker_app_repos_api
    INTERFACE
    molartracker_config
)