add_library(molartracker_ui_profile STATIC
    profile_selection_dlg.cpp
    add_profile_dlg.cpp
)

target_include_directories(molartracker_ui_profile
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../../..
)

target_link_libraries(molartracker_ui_profile
    PUBLIC
        Qt6::Widgets
    PRIVATE
        molartracker_ui_commands
)