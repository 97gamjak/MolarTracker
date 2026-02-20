cmake_minimum_required(VERSION 3.20)
project(MolarTracker LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Core)
find_package(CURL REQUIRED)

message(STATUS "CMAKE_SYSTEM_NAME       = ${CMAKE_SYSTEM_NAME}")
message(STATUS "CMAKE_GENERATOR         = ${CMAKE_GENERATOR}")
message(STATUS "CMAKE_CXX_COMPILER_ID   = ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "CMAKE_CXX_COMPILER      = ${CMAKE_CXX_COMPILER}")
message(STATUS "MSVC                    = ${MSVC}")
message(STATUS "WIN32                   = ${WIN32}")

qt_standard_project_setup()

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_compile_options(
        -Wall
        -Wextra
        -Wpedantic
        -Werror

        # correctness
        -Wshadow
        -Wconversion
        -Wsign-conversion
        -Wcast-align
        -Wformat=2
        -Wnull-dereference
        -Wdouble-promotion
        -Wimplicit-fallthrough
        -Woverloaded-virtual
        -Wnon-virtual-dtor
        -Wold-style-cast
        -Wuseless-cast
        -Wduplicated-cond
        -Wduplicated-branches
        -Wlogical-op
        -Wmisleading-indentation

        # lifetime / safety
        -Wuninitialized
        -Wmaybe-uninitialized
        -Winit-self

        # switch needs to check all enum values
        -Wswitch-enum

        -Wno-error=deprecated-declarations
    )
endif()

if (MSVC)
    message(STATUS "Adding MSVC specific compile options")
    add_compile_options(/Zc:preprocessor)
endif()

include(cmake/version.cmake)

add_compile_definitions(
    MOLARTRACKER_VERSION=\"${MOLARTRACKER_VERSION}\"
    MOLARTRACKER_VERSION_FULL=\"${MOLARTRACKER_VERSION_FULL}\"
    MOLARTRACKER_GIT_SHA=\"${MOLARTRACKER_GIT_SHA}\"
    MOLARTRACKER_GIT_DIRTY=${MOLARTRACKER_GIT_DIRTY}

    MOLARTRACKER_DESKTOP_APP_NAME=\"${PROJECT_NAME}\"
    MOLARTRACKER_GIT_TAG=\"${MOLARTRACKER_GIT_TAG}\"
)

add_subdirectory(src)

if(NOT DEFINED WIN32)
    message(STATUS "Configuring desktop file installation target")
    set(MT_EXEC_PATH "${CMAKE_BINARY_DIR}/bin/${PROJECT_NAME}")
    set(DESKTOP_FILE_PATH "${CMAKE_BINARY_DIR}/${PROJECT_NAME}.desktop")

    configure_file(
    "${CMAKE_SOURCE_DIR}/cmake/MolarTracker.desktop.in"
    "${DESKTOP_FILE_PATH}"
    @ONLY
    )

    add_custom_target(install-desktop-dev
    COMMAND ${CMAKE_COMMAND} -E make_directory "$ENV{HOME}/.local/share/applications"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${DESKTOP_FILE_PATH}"
            "$ENV{HOME}/.local/share/applications/${PROJECT_NAME}.desktop"
    VERBATIM
    )
endif()

option(MOLARTRACKER_ENABLE_DOCS OFF)

if(MOLARTRACKER_ENABLE_DOCS)
  include(cmake/doxygen.cmake)
endif()