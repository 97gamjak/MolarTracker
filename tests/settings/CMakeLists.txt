add_executable(tests_settings
    test_general_settings.cpp
    test_logging_settings.cpp
    test_log_viewer_settings.cpp
    test_ui_settings.cpp
    test_settings.cpp
)

target_link_libraries(tests_settings
    PRIVATE
    molartracker_settings
    molartracker_common
    molartracker_json
    json
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(tests_settings)
add_subdirectory(params)
