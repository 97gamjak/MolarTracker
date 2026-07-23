add_executable(tests_ui
    main.cpp
    test_edit_menu.cpp
    test_position_table_model.cpp
    test_side_bar_items.cpp
    test_stock_info_model.cpp
    test_transaction_table_models.cpp
    test_validators.cpp
)

target_link_libraries(tests_ui
    PRIVATE
    molartracker_ui
    molartracker_drafts
    molartracker_config
    molartracker_finance
    molartracker_common
    Qt6::Widgets
    GTest::gtest
)

include(GoogleTest)
gtest_discover_tests(tests_ui
    PROPERTIES ENVIRONMENT "QT_QPA_PLATFORM=offscreen"
    DISCOVERY_MODE PRE_TEST
)
