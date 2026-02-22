add_library(molartracker_orm STATIC
    field_view.cpp
    orm_exception.cpp
)

target_include_directories(molartracker_orm
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

target_link_libraries(molartracker_orm
    PUBLIC
    molartracker_db
)