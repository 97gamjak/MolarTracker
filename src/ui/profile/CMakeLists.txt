add_library(molartracker_profile_ui STATIC
    profile_selection_dlg.cpp
    add_profile_dlg.cpp
)

target_include_directories(molartracker_profile_ui
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../..
)

target_link_libraries(molartracker_profile_ui
    PUBLIC
    Qt6::Widgets
)