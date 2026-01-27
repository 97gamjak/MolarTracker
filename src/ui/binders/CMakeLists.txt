add_library(molartracker_binders_ui STATIC
    undo_redo_binder.cpp
)

target_include_directories(molartracker_binders_ui
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../..
)

target_link_libraries(molartracker_binders_ui
    PUBLIC
    Qt6::Widgets
    PRIVATE
    molartracker_undo_redo_ui
)