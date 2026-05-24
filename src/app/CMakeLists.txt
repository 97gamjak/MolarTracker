add_library(molartracker_app STATIC
    src/app/service_container.cpp
    src/app/store_container.cpp
    src/app/app_context.cpp

    src/app/services/account_service.cpp
    src/app/services/profile_service.cpp
    src/app/services/transaction_service.cpp
    src/app/services/instrument_service.cpp
    src/app/services/position_service.cpp

    src/app/store/transaction_store.cpp
    src/app/store/stock_store.cpp
    src/app/store/position_store.cpp
    src/app/store/account/account_session.cpp
    src/app/store/account/account_store.cpp
    src/app/store/profile/profile_store.cpp
    src/app/store/profile/active_profile.cpp
    src/app/store/profile/exception.cpp
)

target_include_directories(molartracker_app
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include/
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/
)

target_link_libraries(molartracker_app
    PUBLIC
    molartracker_finance
    molartracker_connections
    PRIVATE
    molartracker_logging
    molartracker_settings
    molartracker_config
    molartracker_drafts
    molartracker_domain
    molartracker_repo
)
