qt_add_library(molartracker_ui_widgets INTERFACE)

add_subdirectory(exceptions)
add_subdirectory(logging)
add_subdirectory(menu_bar)
add_subdirectory(profile)
add_subdirectory(side_bar)
add_subdirectory(utils)
add_subdirectory(validators)

target_link_libraries(molartracker_ui_widgets
    INTERFACE
        Qt6::Widgets
        molartracker_app
        molartracker_ui_commands
        molartracker_ui_exceptions
        molartracker_ui_logging
        molartracker_ui_menu_bar
        molartracker_ui_profile
        molartracker_ui_side_bar
        molartracker_ui_utils
        molartracker_ui_validators
)