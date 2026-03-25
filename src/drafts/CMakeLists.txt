add_library(molartracker_drafts INTERFACE)

target_include_directories(molartracker_drafts
    INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(molartracker_drafts
    INTERFACE
    molartracker_config
)