qt_add_library(molartracker_ui STATIC
    application.cpp
    main_window.cpp
)

add_subdirectory(commands)
add_subdirectory(widgets)

target_link_libraries(molartracker_ui
    PRIVATE
    Qt6::Widgets
    molartracker_ui_commands
    molartracker_ui_widgets
)

target_include_directories(molartracker_ui
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)