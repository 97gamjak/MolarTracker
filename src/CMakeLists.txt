qt_add_executable(MolarTracker
    main.cpp
)

add_library(json INTERFACE)

target_include_directories(json
    INTERFACE
        ${CMAKE_SOURCE_DIR}/external/json/single_include
)

add_library(mstd INTERFACE)

target_include_directories(mstd
    INTERFACE
        ${CMAKE_SOURCE_DIR}/external/mstd/include
)

# 1) Build the generator tool
add_executable(logcat_generator
    ${CMAKE_SOURCE_DIR}/tools/generate_log_categories.cpp
)

# 2) Where to write the generated header
set(GEN_DIR  ${CMAKE_SOURCE_DIR}/logging/generated)
set(GEN_HDR  ${GEN_DIR}/log_categories.gen.hpp)

# 3) Collect inputs (so it reruns when sources change)
file(GLOB_RECURSE LOGCAT_INPUTS CONFIGURE_DEPENDS
    ${CMAKE_SOURCE_DIR}/src/*.cpp
    ${CMAKE_SOURCE_DIR}/src/*.hpp
    ${CMAKE_SOURCE_DIR}/src/*.tpp
)

# 4) Generate header
add_custom_command(
    OUTPUT ${GEN_HDR}
    COMMAND ${CMAKE_COMMAND} -E make_directory ${GEN_DIR}
    COMMAND $<TARGET_FILE:logcat_generator> ${GEN_HDR} ${CMAKE_SOURCE_DIR}/src
    DEPENDS logcat_generator ${LOGCAT_INPUTS}
    VERBATIM
    COMMENT "Generating log categories: ${GEN_HDR}"
)

# 5) Wrapper target (OPTIONAL: add ALL if you want it on every build)
add_custom_target(generate_log_categories
    DEPENDS ${GEN_HDR}    
)

add_library(molartracker_log_categories_generated INTERFACE)
target_include_directories(molartracker_log_categories_generated INTERFACE ${GEN_DIR})
target_sources(molartracker_log_categories_generated INTERFACE ${GEN_HDR})

add_subdirectory(config)
add_subdirectory(logging)
add_subdirectory(utils)
add_subdirectory(drafts)
add_subdirectory(sql_models)
add_subdirectory(db)
add_subdirectory(orm)
add_subdirectory(app)
add_subdirectory(ui)
add_subdirectory(exceptions)
add_subdirectory(settings)
add_subdirectory(json)
add_subdirectory(connections)
add_subdirectory(finance)

target_link_libraries(MolarTracker
    PUBLIC 
    molartracker_ui
    molartracker_app
    molartracker_db
    molartracker_config
    molartracker_drafts
    molartracker_orm
    molartracker_sql_models
    molartracker_logging
    molartracker_utils
    molartracker_exceptions
    molartracker_settings
    molartracker_json
    molartracker_connections
    molartracker_finance
    molartracker_log_categories_generated
)

set_target_properties(MolarTracker PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY
        ${CMAKE_BINARY_DIR}/bin
)

if (WIN32)
    set_target_properties(MolarTracker PROPERTIES
        WIN32_EXECUTABLE ON
    )
endif()

set_target_properties(MolarTracker PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY
    ${CMAKE_BINARY_DIR}/bin
)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(MolarTracker PRIVATE
        -Wall
        -Wextra
        -Wpedantic
        -Werror

        # correctness
        -Wshadow
        -Wconversion
        -Wsign-conversion
        -Wcast-align
        -Wformat=2
        -Wnull-dereference
        -Wdouble-promotion
        -Wimplicit-fallthrough
        -Woverloaded-virtual
        -Wnon-virtual-dtor
        -Wold-style-cast
        -Wuseless-cast
        -Wduplicated-cond
        -Wduplicated-branches
        -Wlogical-op
        -Wmisleading-indentation

        # lifetime / safety
        -Wuninitialized
        -Wmaybe-uninitialized
        -Winit-self

        # switch needs to check all enum values
        -Wswitch-enum

        -Wno-error=deprecated-declarations
    )
endif()