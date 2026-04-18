file(GLOB_RECURSE SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)

# include also .hpp files for moc, since some classes with signals/slots may be defined in headers
file(GLOB_RECURSE HEADER_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp
)

qt_add_library(molartracker_ui STATIC
    ${SOURCE_FILES}
    ${HEADER_FILES}
)

target_link_libraries(molartracker_ui
    PRIVATE
    Qt6::Widgets                # for QApplication
    molartracker_config         # for constants
    molartracker_utils
    molartracker_logging
    molartracker_drafts
    molartracker_finance # TODO(97gamjak): should probably be removed
)

target_include_directories(molartracker_ui
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)