add_library(molartracker_infra STATIC
    database.cpp
)

target_include_directories(molartracker_infra
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

find_package(SQLite3 REQUIRED)

target_link_libraries(molartracker_infra
    PUBLIC
    SQLite::SQLite3
)