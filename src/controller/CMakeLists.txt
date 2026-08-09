qt_add_library(molartracker_controller STATIC

    # general controllers
    src/controller/account_controller.cpp
    src/controller/transaction_controller.cpp
    src/controller/position_controller.cpp
    src/controller/vcs_controller.cpp

    # menu bar
    src/controller/menu_bar/menu_bar_controller.cpp
    src/controller/menu_bar/file_menu_controller.cpp
    src/controller/menu_bar/debug_menu_controller.cpp
    src/controller/menu_bar/settings_menu_controller.cpp
    src/controller/menu_bar/help_menu_controller.cpp
    src/controller/menu_bar/edit_menu_controller.cpp

    src/controller/ensure_profile_controller.cpp

    # side bar
    src/controller/side_bar/side_bar_controller.cpp
    src/controller/side_bar/side_bar_category_controller.cpp
    src/controller/side_bar/account_controller.cpp
    src/controller/side_bar/transaction_controller.cpp
    src/controller/side_bar/securities_controller.cpp

    src/controller/central_controller.cpp

    # handler TODO: refactor this general handler
    src/controller/handlers/handlers.cpp
    src/controller/handlers/logging_handler.cpp
    src/controller/handlers/dirty_handler.cpp

    # transaction
    src/controller/helpers.cpp

    src/controller/main_controller.cpp
)

target_include_directories(molartracker_controller
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(molartracker_controller
    PRIVATE
    Qt6::Widgets
    Qt6::Concurrent
    molartracker_commands
    molartracker_connections
    molartracker_drafts
    molartracker_gateway
    molartracker_http
    molartracker_logging
    molartracker_mapper
    molartracker_settings
    molartracker_service
    molartracker_store
    molartracker_ui
    molartracker_db
    molartracker_vcs
)