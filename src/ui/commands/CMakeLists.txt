add_library(molartracker_commands_ui STATIC
    undo_stack.cpp
    add_profile_command.cpp
)

target_include_directories(molartracker_commands_ui
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../..
)

target_link_libraries(molartracker_commands_ui
    PUBLIC
    Qt6::Core
    PRIVATE
    molartracker_logging
)