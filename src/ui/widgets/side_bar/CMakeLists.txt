add_library(molartracker_ui_side_bar STATIC
    side_bar.cpp
    i_category.cpp
    
    account_category.cpp
    overview_category.cpp
)

target_include_directories(molartracker_ui_side_bar
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../
)

target_link_libraries(molartracker_ui_side_bar
    PRIVATE
    Qt6::Widgets
)