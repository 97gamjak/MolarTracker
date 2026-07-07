add_executable(tests_logging
    test_log_file_cleaner.cpp
)

target_link_libraries(tests_logging
    PRIVATE
    molartracker_logging
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(tests_logging)
