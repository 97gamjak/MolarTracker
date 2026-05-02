set(SRC src/sql_models/)

add_library(molartracker_sql_models STATIC
    ${SRC}/account_row.cpp
    ${SRC}/instrument_row.cpp
    ${SRC}/profile_row.cpp
    ${SRC}/transaction_entry_row.cpp
    ${SRC}/stock_row.cpp
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