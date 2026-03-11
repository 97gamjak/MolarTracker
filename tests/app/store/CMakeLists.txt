add_executable(tests_app_store_profile
  test_profile_store.cpp
)

target_link_libraries(tests_app_store_profile
  PRIVATE
    molartracker_app_store
    molartracker_app_services_api
    molartracker_drafts
    molartracker_connections
    GTest::gtest_main
    GTest::gmock
)

include(GoogleTest)
gtest_discover_tests(tests_app_store_profile)
