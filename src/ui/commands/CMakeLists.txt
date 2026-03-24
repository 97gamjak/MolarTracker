add_library(molartracker_ui_commands STATIC
    undo_stack.cpp
    command_error.cpp
    update_debug_flags_command.cpp
    commands.cpp

    account/create_account_command.cpp

    profile/add_profile_command.cpp
    profile/add_profile_command_error.cpp
    profile/set_active_profile_command.cpp
    profile/set_default_profile_command.cpp
)


target_include_directories(molartracker_ui_commands
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../..
)

target_link_libraries(molartracker_ui_commands
    PRIVATE
    Qt6::Core
    molartracker_logging
    molartracker_settings
)