add_library(molartracker_json STATIC
)

target_include_directories(molartracker_json
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

target_link_libraries(molartracker_json
    PRIVATE
        json
)