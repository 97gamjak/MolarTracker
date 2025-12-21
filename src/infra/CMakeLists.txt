add_library(molartracker_infra STATIC
    database.cpp
)

target_include_directories(molartracker_infra
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

if (WIN32)
    find_package(unofficial-sqlite3 CONFIG REQUIRED)
    target_link_libraries(MolarTracker PRIVATE unofficial::sqlite3::sqlite3)
else()
    find_package(SQLite3 REQUIRED)
endif()

target_link_libraries(molartracker_infra
    PUBLIC
    SQLite::SQLite3
)