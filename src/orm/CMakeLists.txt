add_library(molartracker_orm STATIC
    src/orm/orm_exception.cpp
    src/orm/where_expr.cpp
    src/orm/where_clause.cpp
    src/orm/join.cpp
    src/orm/crud.cpp

    src/orm/crud/crud_error.cpp
)

target_include_directories(molartracker_orm
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(molartracker_orm
    PUBLIC
    molartracker_exceptions
    molartracker_config
    molartracker_logging
    molartracker_filter
    PRIVATE
    molartracker_db
    mstd
)