qt_add_executable(MolarTracker
    main.cpp
)

add_subdirectory(config)
add_subdirectory(logging)
add_subdirectory(utils)
add_subdirectory(drafts)
add_subdirectory(sql_models)
add_subdirectory(db)
add_subdirectory(orm)
add_subdirectory(app)
add_subdirectory(ui)
add_subdirectory(exceptions)
add_subdirectory(settings)

target_link_libraries(MolarTracker
    PRIVATE 
    molartracker_ui
    molartracker_app
    molartracker_db
    molartracker_config
    molartracker_drafts
    molartracker_orm
    molartracker_sql_models
    molartracker_logging
    molartracker_utils
    molartracker_exceptions
    molartracker_settings
)

set_target_properties(MolarTracker PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY
        ${CMAKE_BINARY_DIR}/bin
)

if (WIN32)
    set_target_properties(MolarTracker PROPERTIES
        WIN32_EXECUTABLE ON
    )
endif()

set_target_properties(MolarTracker PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY
    ${CMAKE_BINARY_DIR}/bin
)