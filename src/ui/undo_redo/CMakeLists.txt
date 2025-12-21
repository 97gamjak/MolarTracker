qt_add_library(undo_redo_ui
    undo_stack.cpp
)

target_include_directories(undo_redo_ui
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)