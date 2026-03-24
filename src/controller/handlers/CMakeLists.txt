add_library(molartracker_handlers STATIC
    handlers.cpp
    logging_handler.cpp
    dirty_handler.cpp
)

target_include_directories(molartracker_handlers
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../..
)

target_link_libraries(molartracker_handlers
    PRIVATE
    Qt6::Widgets
    molartracker_connections
    molartracker_settings
)