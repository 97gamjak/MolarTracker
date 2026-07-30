add_executable(tests_settings_params
    test_param_error.cpp
    test_param_container.cpp
    test_param_core.cpp
    test_numeric_param.cpp
    test_enum_param.cpp
    test_numeric_vec_param.cpp
    test_param_container_mixin.cpp
    test_param_utils.cpp
)

target_link_libraries(tests_settings_params
    PRIVATE
        molartracker_settings
        molartracker_exceptions
        molartracker_common
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(tests_settings_params)
