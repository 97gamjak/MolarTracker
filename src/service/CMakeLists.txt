add_library(molartracker_service STATIC
    src/service/service_container.cpp

    src/service/account_service.cpp
    src/service/profile_service.cpp
    src/service/transaction_service.cpp
    src/service/instrument_service.cpp
    src/service/position_service.cpp
)

target_include_directories(molartracker_service
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include/
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/
)

target_link_libraries(molartracker_service
    PUBLIC
    molartracker_finance # TODO: remove this form here
    PRIVATE
    molartracker_logging
    molartracker_repo
)
