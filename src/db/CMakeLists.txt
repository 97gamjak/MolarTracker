add_library(molartracker_db STATIC
    database.cpp
    statement.cpp
)

target_include_directories(molartracker_db
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

if (WIN32)
    find_package(unofficial-sqlite3 CONFIG REQUIRED)
    target_link_libraries(molartracker_db 
        PUBLIC
        unofficial::sqlite3::sqlite3
    )
else()
    find_package(SQLite3 REQUIRED)
    target_link_libraries(molartracker_db
        PUBLIC
        SQLite::SQLite3
    )
endif()

