find_package(Qt6 REQUIRED COMPONENTS Concurrent)

file(GLOB_RECURSE SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)

file(GLOB_RECURSE HEADER_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp
)

qt_add_library(molartracker_vcs STATIC
    ${SOURCE_FILES}
    ${HEADER_FILES}
)

target_include_directories(molartracker_vcs
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(molartracker_vcs
    PUBLIC
    molartracker_http
    molartracker_common
    PRIVATE
    molartracker_logging
    Qt6::Core
    Qt6::Concurrent
    json
)
