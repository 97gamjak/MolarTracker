add_library(molartracker_finance STATIC
    cash.cpp
    currency_exception.cpp
)

target_include_directories(molartracker_finance
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

target_link_libraries(molartracker_finance
    PUBLIC
    mstd
)