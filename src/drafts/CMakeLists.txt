add_library(molartracker_drafts STATIC

    src/drafts/account_draft.cpp
    src/drafts/profile_draft.cpp
    src/drafts/stock_draft.cpp
    
    src/drafts/position/position_draft.cpp
    src/drafts/position/position_stock_draft.cpp

    src/drafts/transaction/transaction_draft.cpp
    src/drafts/transaction/transaction_overview_draft.cpp
    src/drafts/transaction/transaction_create_draft.cpp
)

target_include_directories(molartracker_drafts
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(molartracker_drafts
    PUBLIC
    molartracker_config
    molartracker_utils
    PRIVATE
    molartracker_domain
    molartracker_finance
    molartracker_logging
    molartracker_exceptions
)