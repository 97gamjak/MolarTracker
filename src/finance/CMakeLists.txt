set(SOURCE_DIR
    src/finance
)

add_library(molartracker_finance STATIC
    ${SOURCE_DIR}/ticker_info.cpp
    ${SOURCE_DIR}/yf_client.cpp

    ${SOURCE_DIR}/position.cpp
    ${SOURCE_DIR}/positions.cpp

    ${SOURCE_DIR}/price_quote.cpp
    ${SOURCE_DIR}/price_cache.cpp

    ${SOURCE_DIR}/transaction/cash_transaction.cpp
    ${SOURCE_DIR}/transaction/domain_transaction.cpp
    ${SOURCE_DIR}/transaction/option_data.cpp
    ${SOURCE_DIR}/transaction/pnl.cpp
    ${SOURCE_DIR}/transaction/option_transaction.cpp
    ${SOURCE_DIR}/transaction/security_transaction.cpp
    ${SOURCE_DIR}/transaction/stock_data.cpp
    ${SOURCE_DIR}/transaction/stock_transaction.cpp
    ${SOURCE_DIR}/transaction/trade_leg.cpp
    ${SOURCE_DIR}/transaction/transaction.cpp
    ${SOURCE_DIR}/transaction/transactions.cpp
    ${SOURCE_DIR}/transaction/transaction_converter.cpp
    ${SOURCE_DIR}/transaction/transaction_entry.cpp
    ${SOURCE_DIR}/transaction/transaction_entries.cpp
    ${SOURCE_DIR}/transaction/transaction_filter.cpp

    ${SOURCE_DIR}/instrument/instrument_predicates.cpp
    ${SOURCE_DIR}/instrument/option.cpp
    ${SOURCE_DIR}/instrument/options.cpp
    ${SOURCE_DIR}/instrument/stock.cpp
    ${SOURCE_DIR}/instrument/stocks.cpp

    ${SOURCE_DIR}/account/account.cpp
    ${SOURCE_DIR}/account/accounts.cpp
)

target_include_directories(molartracker_finance
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(molartracker_finance
    PUBLIC
    mstd
    molartracker_filter
    molartracker_http   # TODO(97gamjak): try to move this dependency to private
    molartracker_connections
    molartracker_error
    molartracker_config
    PRIVATE
    molartracker_exceptions
    molartracker_common
    molartracker_logging
    molartracker_json
    mstd
)