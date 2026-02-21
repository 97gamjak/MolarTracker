add_library(molartracker_ui_commands STATIC
    undo_stack.cpp
    command_error.cpp
    update_debug_flags_command.cpp
    commands.cpp
)

add_subdirectory(profile)

target_include_directories(molartracker_ui_commands
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../..
)

target_link_libraries(molartracker_ui_commands
    PUBLIC
    Qt6::Core
    molartracker_ui_commands_profile
    PRIVATE
    molartracker_logging
)