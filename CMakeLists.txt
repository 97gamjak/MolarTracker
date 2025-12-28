cmake_minimum_required(VERSION 3.20)
project(MolarTracker LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

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
        -Wswitch-default

        -Wno-error=deprecated-declarations
    )
endif()

add_subdirectory(src)
