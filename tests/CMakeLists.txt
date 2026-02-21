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

add_subdirectory(db)