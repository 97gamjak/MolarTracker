add_library(molartracker_ui_menu_bar STATIC
    menu_bar.cpp
    debug_menu.cpp
    file_menu.cpp
    settings_menu.cpp
    help_menu.cpp
    edit_menu.cpp
)

target_include_directories(molartracker_ui_menu_bar
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../
)

target_link_libraries(molartracker_ui_menu_bar
    PRIVATE
    Qt6::Widgets
)