add_library(molartracker_json STATIC
)

target_include_directories(molartracker_json
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${PROJECT_SOURCE_DIR}/external/json/single_include
)