set(SOURCE_DIR
    src/gateway
)

add_library(molartracker_gateway STATIC
    ${SOURCE_DIR}/position_gateway.cpp
)

target_include_directories(molartracker_gateway
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(molartracker_gateway
    PUBLIC
    PRIVATE
    molartracker_finance
    molartracker_store
    molartracker_config
    molartracker_utils
)