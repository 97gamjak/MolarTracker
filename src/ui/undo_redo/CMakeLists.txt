add_library(molartracker_undo_redo_ui STATIC
    undo_stack.cpp
)

target_include_directories(molartracker_undo_redo_ui
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../..
)