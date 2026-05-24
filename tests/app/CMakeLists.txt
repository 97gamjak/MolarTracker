add_executable(tests_app
    test_account_repo.cpp
    test_account_service.cpp
    test_instrument_repo.cpp
    test_instrument_service.cpp
    test_position_service.cpp
    test_profile_repo.cpp
    test_profile_service.cpp
    test_transaction_repo.cpp
    test_transaction_service.cpp
)

target_include_directories(tests_app
    PRIVATE
    ${CMAKE_SOURCE_DIR}/src/app/src/
)

target_link_libraries(tests_app
    PRIVATE
    molartracker_app
    molartracker_db
    molartracker_domain
    molartracker_logging
    molartracker_sql_models
    molartracker_tests
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(tests_app)

add_subdirectory(store)
