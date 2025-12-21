qt_add_executable(MolarTracker
    main.cpp
)

add_subdirectory(ui)

target_link_libraries(MolarTracker
    PRIVATE molartracker_ui
)

if (WIN32)
    find_package(unofficial-sqlite3 CONFIG REQUIRED)
    target_link_libraries(MolarTracker PRIVATE unofficial::sqlite3::sqlite3)

    set_target_properties(MolarTracker PROPERTIES
        WIN32_EXECUTABLE ON
    )
else()
    find_package(SQLite3 REQUIRED)
endif()

set_target_properties(MolarTracker PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY
    ${CMAKE_BINARY_DIR}/bin
)