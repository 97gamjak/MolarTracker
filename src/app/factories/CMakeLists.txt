add_library(molartracker_app_factories STATIC
    profile_factory.cpp
    account_factory.cpp
)

target_link_libraries(molartracker_app_factories
    PRIVATE
    molartracker_app_domain
    molartracker_config
    molartracker_sql_models
)

target_include_directories(molartracker_app_factories
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)