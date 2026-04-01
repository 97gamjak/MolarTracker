add_library(molartracker_finance STATIC
    src/finance/cash.cpp
    src/finance/account.cpp
    src/finance/currency_exception.cpp
    src/finance/transaction.cpp
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
    PRIVATE
    molartracker_config
    molartracker_exceptions
    molartracker_utils
)