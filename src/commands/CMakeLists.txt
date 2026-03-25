file(GLOB_RECURSE SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)

# include also .hpp files for moc, since some classes with signals/slots may be defined in headers
file(GLOB_RECURSE HEADER_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp
)

qt_add_library(molartracker_commands STATIC
    ${SOURCE_FILES}
    ${HEADER_FILES}
)

target_link_libraries(molartracker_commands
    PRIVATE
    Qt6::Widgets
    molartracker_logging
    molartracker_drafts
    molartracker_app
    molartracker_settings
)

target_include_directories(molartracker_commands
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)