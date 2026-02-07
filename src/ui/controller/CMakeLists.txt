add_library(molartracker_ui_controller STATIC
    file_menu_controller.cpp
    debug_menu_controller.cpp
    settings_menu_controller.cpp
    help_menu_controller.cpp
    edit_menu_controller.cpp
    ensure_profile_controller.cpp
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
        molartracker_app_repos_api
        molartracker_logging
)