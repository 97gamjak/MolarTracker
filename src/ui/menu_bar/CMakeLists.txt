add_library(molartracker_menu_bar_ui STATIC
    menu_bar.cpp
    debug_menu.cpp
    file_menu.cpp
    settings_menu.cpp
    help_menu.cpp
)

target_include_directories(molartracker_menu_bar_ui
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../..
)

target_link_libraries(molartracker_menu_bar_ui
    PRIVATE
    Qt6::Widgets
)