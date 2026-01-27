qt_add_library(molartracker_ui STATIC
    application.cpp
    main_window.cpp
)

add_subdirectory(undo_redo)
add_subdirectory(profile)
add_subdirectory(exceptions)
add_subdirectory(binders)
add_subdirectory(menu_bar)

target_link_libraries(molartracker_ui
    PUBLIC
        Qt6::Widgets
    PRIVATE
        molartracker_undo_redo_ui
        molartracker_profile_ui
        molartracker_exceptions_ui
        molartracker_app
        molartracker_binders_ui
        molartracker_menu_bar_ui
)

target_include_directories(molartracker_ui
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)