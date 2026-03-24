add_library(molartracker_ui_widgets_account STATIC
    create_account_dlg.cpp
)

target_include_directories(molartracker_ui_widgets_account
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../../..
)

target_link_libraries(molartracker_ui_widgets_account
    PRIVATE
    Qt6::Widgets
    molartracker_logging
)