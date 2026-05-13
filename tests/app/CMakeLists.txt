add_executable(tests_app
    test_account_repo.cpp
    test_profile_repo.cpp
)

target_include_directories(tests_app
    PRIVATE
    ${CMAKE_SOURCE_DIR}/src/app/src/
)

target_link_libraries(tests_app
    PRIVATE
    molartracker_app
    molartracker_db
    molartracker_logging
    molartracker_sql_models
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(tests_app)
