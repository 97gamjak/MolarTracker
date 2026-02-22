add_library(molartracker_ui_validators STATIC
    line_validator.cpp
    name_line_edit.cpp
    email_line_edit.cpp
)

target_include_directories(molartracker_ui_validators
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../../..
)

target_link_libraries(molartracker_ui_validators
    PRIVATE
        Qt6::Widgets
)