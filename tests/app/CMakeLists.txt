add_executable(tests_app
    test_transaction_repo.cpp
)

target_link_libraries(tests_app
    PRIVATE
        molartracker_app
        molartracker_db
        molartracker_logging
        GTest::gtest_main
)

target_include_directories(tests_app
    PRIVATE
        ${CMAKE_SOURCE_DIR}/src/app/src
)

include(GoogleTest)
gtest_discover_tests(tests_app)
