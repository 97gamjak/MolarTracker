add_library(molartracker_domain STATIC
    src/domain/profile.cpp
    src/domain/profile_mapper.cpp
)

target_include_directories(molartracker_domain
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(molartracker_domain
    PRIVATE
    molartracker_config
    molartracker_drafts
    molartracker_filter
)