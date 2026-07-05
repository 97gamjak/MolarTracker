add_library(molartracker_cache STATIC
    src/cache/cache_container.cpp

    src/cache/base_cache.cpp
    
    src/cache/account_cache.cpp
    src/cache/account_cache_utils.cpp
    src/cache/stock_cache.cpp
    src/cache/stock_cache_utils.cpp
)

target_include_directories(molartracker_cache
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include/
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/
)

target_link_libraries(molartracker_cache
    PRIVATE
    molartracker_store
    molartracker_finance
    molartracker_utils
    molartracker_config
    molartracker_logging
    molartracker_settings
)