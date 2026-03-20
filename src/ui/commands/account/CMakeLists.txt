add_library(molartracker_ui_commands_account STATIC
    create_account_command.cpp
)

target_include_directories(molartracker_ui_commands_account
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../../..
)

target_link_libraries(molartracker_ui_commands_account
    PUBLIC
    mstd
    json
)