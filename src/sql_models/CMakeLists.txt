add_library(molartracker_sql_models INTERFACE)

target_include_directories(molartracker_sql_models
    INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

target_link_libraries(molartracker_sql_models
    INTERFACE
    molartracker_config
    molartracker_orm
)