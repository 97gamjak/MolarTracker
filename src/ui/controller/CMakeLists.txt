add_library(molartracker_controller_ui STATIC
    file_menu_controller.cpp
    debug_menu_controller.cpp
    settings_menu_controller.cpp
    help_menu_controller.cpp
    edit_menu_controller.cpp
)

target_include_directories(molartracker_controller_ui
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../..
)

target_link_libraries(molartracker_controller_ui
    PRIVATE
        Qt6::Widgets
        molartracker_commands_ui
        molartracker_logging_ui
        molartracker_app_repos_api
        molartracker_logging
)