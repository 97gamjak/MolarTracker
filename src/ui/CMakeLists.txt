file(GLOB_RECURSE SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)

# include also .hpp files for moc, since some classes with signals/slots may be defined in headers
file(GLOB_RECURSE HEADER_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp
)

# Qt resource collections (e.g. help screenshots), auto-compiled by AUTORCC
file(GLOB_RECURSE RESOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/*.qrc
)

qt_add_library(molartracker_ui STATIC
    ${SOURCE_FILES}
    ${HEADER_FILES}
    ${RESOURCE_FILES}
)

target_link_libraries(molartracker_ui
    PRIVATE
    Qt6::Widgets                # for QApplication
    Qt6::PrintSupport           # for QPrinter / PDF export
    molartracker_config         # for constants
    molartracker_http           # for HttpClient::urlEncode
    molartracker_common
    molartracker_logging
    molartracker_drafts
    molartracker_settings
)

target_include_directories(molartracker_ui
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)
