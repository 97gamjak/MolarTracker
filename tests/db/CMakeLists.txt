add_executable(tests_db
  test_database.cpp
  test_statement.cpp
  test_transaction.cpp
)

target_link_libraries(tests_db
  PRIVATE
    molartracker_db
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(tests_db)