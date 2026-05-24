add_executable(tests_app_store
    test_account_store.cpp
    test_position_store.cpp
    test_profile_store.cpp
    test_stock_store.cpp
    test_transaction_store.cpp
)

target_include_directories(tests_app_store
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src/app/src/
)

target_link_libraries(tests_app_store
    PRIVATE
    molartracker_app
    molartracker_domain
    molartracker_drafts
    molartracker_logging
    molartracker_service
    molartracker_tests
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(tests_app_store)
