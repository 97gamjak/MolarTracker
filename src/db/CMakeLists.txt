add_library(molartracker_db STATIC
    src/db/backup_manager.cpp
    src/db/database.cpp
    src/db/db_exception.cpp
    src/db/statement.cpp
    src/db/transaction.cpp
)

target_include_directories(molartracker_db
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(molartracker_db
    PRIVATE
    molartracker_config
    molartracker_logging
    molartracker_settings
    molartracker_common
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

