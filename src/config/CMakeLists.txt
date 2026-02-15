add_library(molartracker_config STATIC
    constants.cpp
)

target_include_directories(molartracker_config
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

target_link_libraries(molartracker_config
    PRIVATE
    molartracker_utils
    molartracker_exceptions
)