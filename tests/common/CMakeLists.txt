add_executable(tests_common
  test_paths.cpp
  test_version.cpp
)

target_link_libraries(tests_common
  PRIVATE
    molartracker_common
    GTest::gtest_main
    json
)

include(GoogleTest)
gtest_discover_tests(tests_common)
