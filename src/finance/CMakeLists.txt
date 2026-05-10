add_library(molartracker_finance STATIC
    src/finance/cash.cpp
    src/finance/account.cpp
    src/finance/currency.cpp
    src/finance/currency_exception.cpp
    
    src/finance/transaction.cpp
    src/finance/transaction_entry.cpp
    src/finance/trade_data.cpp
    src/finance/stock.cpp
    src/finance/ticker_info.cpp
    src/finance/yf_client.cpp
    src/finance/position.cpp
    src/finance/finance_error.cpp
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
    PRIVATE
    molartracker_config
    molartracker_exceptions
    molartracker_utils
    molartracker_logging
    mstd
)