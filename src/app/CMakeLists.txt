add_library(molartracker_app STATIC
    repo_container.cpp
    service_container.cpp
    store_container.cpp
    app_context.cpp
)

add_subdirectory(repos)
add_subdirectory(services)
add_subdirectory(domain)
add_subdirectory(services_api)
add_subdirectory(repos_api)
add_subdirectory(store)

target_include_directories(molartracker_app
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

target_link_libraries(molartracker_app
    PUBLIC
    molartracker_db
    molartracker_app_repos
    molartracker_app_services
    molartracker_app_domain
    molartracker_app_store
)
