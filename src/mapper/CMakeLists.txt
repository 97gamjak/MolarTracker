set(SOURCE_DIR
    src/mapper
)

add_library(molartracker_mapper STATIC
    ${SOURCE_DIR}/account_mapper.cpp
    ${SOURCE_DIR}/profile_mapper.cpp
    ${SOURCE_DIR}/stock_mapper.cpp
    ${SOURCE_DIR}/option_mapper.cpp

    ${SOURCE_DIR}/transaction/transaction_mapper.cpp
    ${SOURCE_DIR}/transaction/transaction_overview_mapper.cpp
    ${SOURCE_DIR}/transaction/transaction_create_mapper.cpp
)

target_include_directories(molartracker_mapper
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(molartracker_mapper
    PUBLIC
    PRIVATE
    molartracker_domain
    molartracker_drafts
    molartracker_finance
    molartracker_logging
)