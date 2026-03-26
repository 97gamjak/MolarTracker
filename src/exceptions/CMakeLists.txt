add_library(molartracker_exceptions STATIC
    src/exceptions/base.cpp
    src/exceptions/file_exceptions.cpp
)

target_include_directories(molartracker_exceptions
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)