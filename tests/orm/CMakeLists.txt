add_executable(tests_orm
  test_binder.cpp
)

target_link_libraries(tests_orm
  PRIVATE
    molartracker_orm
    GTest::gtest_main
    mstd
)

include(GoogleTest)
gtest_discover_tests(tests_orm)