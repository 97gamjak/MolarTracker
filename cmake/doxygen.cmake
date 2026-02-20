find_package(Doxygen REQUIRED)

set(DOXYGEN_OUTPUT_DIR "${CMAKE_BINARY_DIR}/doxygen")

set(DOXYGEN_PROJECT_NAME    "${PROJECT_NAME}")
set(DOXYGEN_PROJECT_VERSION "${PROJECT_VERSION}")

set(DOXYGEN_INPUT
    "${PROJECT_SOURCE_DIR}/src"
)

set(DOXYFILE_IN  "${PROJECT_SOURCE_DIR}/docs/Doxyfile.in")
set(DOXYFILE_OUT "${CMAKE_BINARY_DIR}/Doxyfile")

configure_file("${DOXYFILE_IN}" "${DOXYFILE_OUT}" @ONLY)

add_custom_target(docs
    COMMAND "${DOXYGEN_EXECUTABLE}" "${DOXYFILE_OUT}"
    WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
    COMMENT "Generating Doxygen documentation (STRICT)"
    VERBATIM
)
