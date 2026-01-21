add_library(molartracker_utils STATIC
    timestamp.cpp
)

target_include_directories(molartracker_utils
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)