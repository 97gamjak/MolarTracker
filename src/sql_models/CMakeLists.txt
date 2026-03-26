add_library(molartracker_sql_models STATIC
    src/sql_models/profile_row.cpp
)

target_include_directories(molartracker_sql_models
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(molartracker_sql_models
    PRIVATE
    molartracker_config
    molartracker_orm
)