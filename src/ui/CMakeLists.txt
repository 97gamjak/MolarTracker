qt_add_library(molartracker_ui
    main_window.cpp
    top_menu_bar.cpp
)

add_subdirectory(undo_redo)

target_link_libraries(molartracker_ui
    PRIVATE
        undo_redo_ui
    PUBLIC
        Qt6::Widgets
)

target_include_directories(molartracker_ui
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)