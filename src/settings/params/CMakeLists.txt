add_library(molartracker_settings_params STATIC
    param_error.cpp
    param_container.cpp
)

target_include_directories(molartracker_settings_params
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../..
    ${CMAKE_PROJECT_DIR}/external/json/single_include
    ${CMAKE_PROJECT_DIR}/external/mstd/include
)

target_link_libraries(molartracker_settings_params
    PUBLIC
)
