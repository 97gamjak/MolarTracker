qt_add_executable(MolarTracker
    main.cpp
)

target_link_libraries(MolarTracker
    PRIVATE molartracker_ui
)

set_target_properties(MolarTracker PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY
        ${CMAKE_BINARY_DIR}/bin
)

add_subdirectory(ui)

if (WIN32)
    set_target_properties(MolarTracker PROPERTIES
        WIN32_EXECUTABLE ON
    )
endif()
