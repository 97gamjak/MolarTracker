include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        v1.17.0
)

# MSVC runtime consistency: recommended by GoogleTest docs/README.
# Prevents LNK2038 RuntimeLibrary mismatch in common setups.
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)  # Windows/MSVC only matters
FetchContent_MakeAvailable(googletest)

add_library(molartracker_tests STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/test_fixtures.cpp
)

target_include_directories(molartracker_tests
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

add_subdirectory(app)
add_subdirectory(db)
add_subdirectory(orm)
add_subdirectory(utils)
add_subdirectory(settings)