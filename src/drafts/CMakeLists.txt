add_library(molartracker_drafts STATIC
    src/drafts/account_mapper.cpp
    src/drafts/transaction_draft.cpp
    src/drafts/transaction_mapper.cpp
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
    molartracker_finance
)