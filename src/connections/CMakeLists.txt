add_library(molartracker_connections STATIC
    connection.cpp
    signal_exception.cpp
)

target_include_directories(molartracker_connections
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)