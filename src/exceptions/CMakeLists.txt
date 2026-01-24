add_library(molartracker_exceptions STATIC
    base.cpp
)

target_include_directories(molartracker_exceptions
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)