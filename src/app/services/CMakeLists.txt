add_library(molartracker_app_services STATIC
    profile_service.cpp
)

target_include_directories(molartracker_app_services
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

target_link_libraries(molartracker_app_services
    PUBLIC
    molartracker_app_services_api
    PRIVATE
    molartracker_app_repos
    molartracker_app_domain
)