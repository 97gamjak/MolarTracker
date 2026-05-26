add_library(molartracker_drafts STATIC
    src/drafts/account/account_draft.cpp

    src/drafts/transaction_draft.cpp
    src/drafts/transaction_mapper.cpp

    src/drafts/stock_draft.cpp
    src/drafts/stock_mapper.cpp

    src/drafts/position_draft.cpp

    src/drafts/profile_draft.cpp
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
    molartracker_exceptions
    molartracker_finance
    molartracker_logging
    molartracker_logic
)