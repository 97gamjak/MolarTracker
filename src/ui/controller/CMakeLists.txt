add_library(molartracker_ui_controller STATIC
    menu_bar/menu_bar_controller.cpp
    menu_bar/file_menu_controller.cpp
    menu_bar/debug_menu_controller.cpp
    menu_bar/settings_menu_controller.cpp
    menu_bar/help_menu_controller.cpp
    menu_bar/edit_menu_controller.cpp

    ensure_profile_controller.cpp

    side_bar_controller.cpp
)

target_include_directories(molartracker_ui_controller
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../..
)

target_link_libraries(molartracker_ui_controller
    PRIVATE
        Qt6::Widgets
        molartracker_ui_widgets
        molartracker_ui_logging
        molartracker_ui_commands
        molartracker_app_repos_api
        molartracker_logging
)