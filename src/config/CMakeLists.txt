add_library(molartracker_config STATIC
    src/config/constants.cpp
)

target_include_directories(molartracker_config
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include/
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/
)

target_link_libraries(molartracker_config
    PRIVATE
    molartracker_utils
    molartracker_exceptions
    PUBLIC
    mstd    # for exposing mstd types in the public header
)

set_property(TARGET molartracker_config APPEND PROPERTY
    AUTOGEN_TARGET_DEPENDS generate_log_categories
)