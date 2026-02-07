# ---- Git-derived version (runs at configure time) ----
find_package(Git QUIET)

set(MOLARTRACKER_VERSION "0.0.0")
set(MOLARTRACKER_VERSION_FULL "0.0.0")
set(MOLARTRACKER_GIT_SHA "unknown")
set(MOLARTRACKER_GIT_DIRTY 0)

if(GIT_FOUND AND EXISTS "${CMAKE_SOURCE_DIR}/.git")
    execute_process(
        COMMAND "${GIT_EXECUTABLE}" describe --tags --long --dirty
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
        OUTPUT_VARIABLE GIT_DESCRIBE
        OUTPUT_STRIP_TRAILING_WHITESPACE
        ERROR_QUIET
    )

    execute_process(
        COMMAND "${GIT_EXECUTABLE}" rev-parse --short=12 HEAD
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
        OUTPUT_VARIABLE GIT_SHA
        OUTPUT_STRIP_TRAILING_WHITESPACE
        ERROR_QUIET
    )

    if(GIT_DESCRIBE)
        # Examples:
        # v1.2.3-0-gabcdef123456
        # v1.2.3-4-gabcdef123456
        # v1.2.3-4-gabcdef123456-dirty

        string(REGEX MATCH "^([0-9]+)\\.([0-9]+)\\.([0-9]+)-([0-9]+)-g([0-9a-f]+)(-dirty)?$"
               _ "${GIT_DESCRIBE}")

        message(STATUS "Git version: ${GIT_DESCRIBE}")

        if(CMAKE_MATCH_COUNT GREATER 0)
            set(V_MAJOR "${CMAKE_MATCH_1}")
            set(V_MINOR "${CMAKE_MATCH_2}")
            set(V_PATCH "${CMAKE_MATCH_3}")
            set(V_AHEAD "${CMAKE_MATCH_4}")
            set(V_HASH  "${CMAKE_MATCH_5}")
            set(V_DIRTY "${CMAKE_MATCH_6}")

            set(MOLARTRACKER_VERSION "${V_MAJOR}.${V_MINOR}.${V_PATCH}")
            if(V_AHEAD STREQUAL "0" AND NOT V_DIRTY)
                set(MOLARTRACKER_VERSION_FULL "${MOLARTRACKER_VERSION}")
            else()
                # You can pick your own convention here:
                # - semver build metadata: 1.2.3+4.gabcdef
                # - or dev suffix:         1.2.3-dev.4+gabcdef
                set(MOLARTRACKER_VERSION_FULL "${MOLARTRACKER_VERSION}+${V_AHEAD}.g${V_HASH}")
                if(V_DIRTY)
                    set(MOLARTRACKER_VERSION_FULL "${MOLARTRACKER_VERSION_FULL}.dirty")
                endif()
            endif()

            if(V_DIRTY)
                set(MOLARTRACKER_GIT_DIRTY 1)
            endif()
        endif()
    endif()

    if(GIT_SHA)
        set(MOLARTRACKER_GIT_SHA "${GIT_SHA}")
    endif()
endif()

# ---- GitHub Actions / CI-provided build metadata ----
# These are meant to be passed via -D... on the CMake command line:
#   -DMOLARTRACKER_IS_TAG=ON|OFF
#   -DMOLARTRACKER_GIT_TAG="1.2.3"   (or empty on non-tag builds)
#
# In your workflow we pass these for every build. For PR/dev builds the tag
# string may be meaningless; the boolean tells you what it is.

option(MOLARTRACKER_IS_TAG "Build was triggered from a Git tag" OFF)

if(NOT DEFINED MOLARTRACKER_GIT_TAG)
    set(MOLARTRACKER_GIT_TAG "")
endif()

# Optional: ensure it is always a string (avoid CMake treating it as a list)
string(REPLACE ";" "" MOLARTRACKER_GIT_TAG "${MOLARTRACKER_GIT_TAG}")
