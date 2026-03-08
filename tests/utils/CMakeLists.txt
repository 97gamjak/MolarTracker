add_executable(tests_utils
  test_paths.cpp
  test_version.cpp
)

target_link_libraries(tests_utils
  PRIVATE
    molartracker_utils
    GTest::gtest_main
    json
)

include(GoogleTest)
gtest_discover_tests(tests_utils)
