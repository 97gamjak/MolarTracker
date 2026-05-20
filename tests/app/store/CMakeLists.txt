add_executable(tests_stores
    test_account_store.cpp
    test_position_store.cpp
    test_profile_store.cpp
    test_stock_store.cpp
    test_transaction_store.cpp
)

target_include_directories(tests_stores
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(tests_stores
    PRIVATE
    molartracker_app
    molartracker_drafts
    molartracker_logging
    molartracker_tests
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(tests_stores)
