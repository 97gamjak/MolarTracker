add_library(molartracker_error STATIC
    src/error/http_error.cpp
)

target_include_directories(molartracker_error
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include/
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/
)

target_link_libraries(molartracker_error
    PUBLIC
    mstd
)