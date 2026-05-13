add_executable(tests_orm
  test_crud.cpp
)

target_link_libraries(tests_orm
  PRIVATE
    molartracker_orm
    molartracker_config
    molartracker_db
    GTest::gtest_main
    mstd
)

include(GoogleTest)
gtest_discover_tests(tests_orm)