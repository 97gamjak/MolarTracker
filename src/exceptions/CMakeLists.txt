add_library(molartracker_exceptions STATIC
    src/exceptions/base.cpp
    src/exceptions/file_exceptions.cpp
    src/exceptions/not_yet_implemented.cpp
)

target_include_directories(molartracker_exceptions
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_link_libraries(molartracker_exceptions PUBLIC stdc++exp)
endif()