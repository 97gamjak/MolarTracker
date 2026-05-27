add_library(molartracker_logic STATIC
    src/logic/finance/account.cpp
)

target_include_directories(molartracker_logic
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(molartracker_logic
    PRIVATE
    molartracker_config
    molartracker_filter
)