add_library(molartracker_orm STATIC
    fixed_string.cpp
)

target_include_directories(molartracker_orm
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)