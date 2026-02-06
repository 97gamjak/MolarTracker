add_library(molartracker_ui_exceptions STATIC
    exception_dialog.cpp
)

target_include_directories(molartracker_ui_exceptions
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../../..
)

target_link_libraries(molartracker_ui_exceptions
    PUBLIC
    Qt6::Widgets
)