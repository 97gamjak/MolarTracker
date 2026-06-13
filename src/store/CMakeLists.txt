add_library(molartracker_store STATIC
    src/store/store_container.cpp

    src/store/option_store.cpp
    src/store/stock_store.cpp
    src/store/transaction_store.cpp

    src/store/position_store.cpp
    src/store/account/account_store.cpp
    src/store/profile/profile_store.cpp
    src/store/profile/active_profile.cpp
    src/store/profile/exception.cpp
)

target_include_directories(molartracker_store
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include/
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/
)

target_link_libraries(molartracker_store
    PUBLIC
    molartracker_finance
    molartracker_connections
    molartracker_domain
    PRIVATE
    molartracker_service
    molartracker_logging
    molartracker_config
)
