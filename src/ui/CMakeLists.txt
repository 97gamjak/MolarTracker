qt_add_library(molartracker_ui STATIC
    application.cpp
    main_window.cpp
)

add_subdirectory(commands)
add_subdirectory(controller)
add_subdirectory(widgets)

target_link_libraries(molartracker_ui
    PUBLIC
        Qt6::Widgets
    PRIVATE
        molartracker_ui_commands
        molartracker_ui_widgets
        molartracker_ui_controller
        molartracker_app
)

target_include_directories(molartracker_ui
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)