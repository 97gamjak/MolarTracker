add_library(molartracker_connections STATIC
    src/connections/connection.cpp
    src/connections/signal_exception.cpp
)

target_include_directories(molartracker_connections
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(molartracker_connections
    PUBLIC
    molartracker_exceptions # for inheriting from exceptions
)