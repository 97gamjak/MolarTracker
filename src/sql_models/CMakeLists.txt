add_library(molartracker_sql_models STATIC
    profile_row.cpp
)

target_include_directories(molartracker_sql_models
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

target_link_libraries(molartracker_sql_models
    PRIVATE
    mstd
)