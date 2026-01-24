add_library(molartracker_exceptions_ui STATIC
    exception_dialog.cpp
)

target_include_directories(molartracker_exceptions_ui
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../..
)

target_link_libraries(molartracker_exceptions_ui
    PUBLIC
    Qt6::Widgets
)