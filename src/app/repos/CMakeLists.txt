add_library(molartracker_app_repos STATIC
    profile_repo.cpp
)

target_link_libraries(molartracker_app_repos
    PUBLIC
    molartracker_app_repos_api
    PRIVATE
    molartracker_db
    molartracker_app_domain
    molartracker_app_factories
)

target_include_directories(molartracker_app_repos
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)