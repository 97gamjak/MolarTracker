add_executable(tests_vcs
  test_asset_selector.cpp
)

target_link_libraries(tests_vcs
  PRIVATE
    molartracker_vcs
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(tests_vcs)
