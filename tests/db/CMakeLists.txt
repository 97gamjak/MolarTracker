add_executable(tests_db_database
  test_database.cpp
  test_statement.cpp
  test_transaction.cpp
)

target_link_libraries(tests_db_database
  PRIVATE
    molartracker_db
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(tests_db_database)