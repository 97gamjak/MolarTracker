add_library(molartracker_store STATIC
    src/store/store_container.cpp

    src/store/transaction_store.cpp
    src/store/stock_store.cpp
    src/store/position_store.cpp

    src/store/account/account_mapper.cpp
    src/store/account/account_session.cpp
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
    molartracker_service # TODO: make this private as soon as store are interfaced
    PRIVATE
    molartracker_config
    molartracker_drafts
    molartracker_domain
    molartracker_logging
    molartracker_logic
)
