add_library(molartracker_http STATIC
    src/http/curl.cpp
    src/http/http_client.cpp
)

target_include_directories(molartracker_http
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(molartracker_http
    PUBLIC
    CURL::libcurl
    PRIVATE
    mstd
)