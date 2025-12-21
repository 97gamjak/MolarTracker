qt_add_executable(MolarTracker
    main.cpp
)

add_subdirectory(config)
add_subdirectory(infra)
add_subdirectory(app)
add_subdirectory(ui)

target_link_libraries(MolarTracker
    PRIVATE 
    molartracker_ui
    molartracker_app
    molartracker_infra
    molartracker_config
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