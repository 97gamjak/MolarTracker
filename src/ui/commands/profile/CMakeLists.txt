add_library(molartracker_ui_commands_profile STATIC
    set_default_profile_command.cpp
    set_active_profile_command.cpp
    add_profile_command.cpp
    add_profile_command_error.cpp
)

target_include_directories(molartracker_ui_commands_profile
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../../..
)

target_link_libraries(molartracker_ui_commands_profile
    PUBLIC
    mstd
    json
)