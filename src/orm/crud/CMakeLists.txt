add_library(molartracker_orm_crud STATIC
    crud_error.cpp
)

target_include_directories(molartracker_orm_crud
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../..
)

target_link_libraries(molartracker_orm_crud
    PUBLIC
    molartracker_db
    mstd
    molartracker_exceptions
)