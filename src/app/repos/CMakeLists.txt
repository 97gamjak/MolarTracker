add_library(molartracker_app_repos STATIC
    profile_repo.cpp
    account_repo.cpp
    repo_errors.cpp
)

target_link_libraries(molartracker_app_repos
    PRIVATE
    molartracker_app_repos_api
    molartracker_app_factories
    molartracker_finance
    molartracker_sql_models
)

target_include_directories(molartracker_app_repos
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)