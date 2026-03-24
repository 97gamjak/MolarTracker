add_library(molartracker_controller STATIC
    menu_bar/menu_bar_controller.cpp
    menu_bar/file_menu_controller.cpp
    menu_bar/debug_menu_controller.cpp
    menu_bar/settings_menu_controller.cpp
    menu_bar/help_menu_controller.cpp
    menu_bar/edit_menu_controller.cpp

    ensure_profile_controller.cpp

    side_bar_controller.cpp
    side_bar_category_controller.cpp
    account_controller.cpp

    main_controller.cpp
)

add_subdirectory(handlers)

target_include_directories(molartracker_controller
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

target_link_libraries(molartracker_controller
    PRIVATE
    Qt6::Widgets
    mstd
    json
    PRIVATE
    molartracker_handlers
    molartracker_app
)