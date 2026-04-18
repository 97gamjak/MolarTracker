add_library(molartracker_json INTERFACE
)

target_include_directories(molartracker_json
    INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(molartracker_json
    INTERFACE
    json
)