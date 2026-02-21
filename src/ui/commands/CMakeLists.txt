add_library(molartracker_ui_commands STATIC
    undo_stack.cpp
    add_profile_command.cpp
    set_default_profile_command.cpp
    add_profile_command_error.cpp
    command_error.cpp
    update_debug_flags_command.cpp
    commands.cpp
)

target_include_directories(molartracker_ui_commands
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../..
)

target_link_libraries(molartracker_ui_commands
    PUBLIC
    Qt6::Core
    PRIVATE
    molartracker_logging
)