add_executable(tests_ui
    main.cpp
    test_validators.cpp
    test_amount_line_edit.cpp
    test_stock_info_model.cpp
    test_log_category_model.cpp
    test_cash_transaction_table.cpp
    test_stock_transaction_table.cpp
    test_sidebar_items.cpp
)

target_link_libraries(tests_ui
    PRIVATE
        molartracker_ui
        molartracker_logging
        molartracker_drafts
        molartracker_finance
        Qt6::Widgets
        Qt6::Test
        GTest::gtest
)

include(GoogleTest)
gtest_discover_tests(tests_ui
    PROPERTIES
        ENVIRONMENT "QT_QPA_PLATFORM=offscreen"
)
