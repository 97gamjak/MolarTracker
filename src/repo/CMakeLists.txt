add_library(molartracker_repo STATIC
    src/repo/repo_container.cpp

    src/repo/account_repo.cpp
    src/repo/profile_repo.cpp
    src/repo/base_repo.cpp
    src/repo/repo_errors.cpp
    src/repo/transaction_repo.cpp
    src/repo/instrument_repo.cpp
    src/repo/position_repo.cpp

    src/repo/factories/account_factory.cpp
    src/repo/factories/profile_factory.cpp
    src/repo/factories/transaction_factory.cpp
    src/repo/factories/instrument_factory.cpp
    src/repo/factories/position_factory.cpp

    src/repo/migration/migration_runner.cpp
    src/repo/migration/migration.cpp
    src/repo/migration/single_migration.cpp
    src/repo/migration/multi_migration.cpp
)

target_include_directories(molartracker_repo
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include/
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/
)

target_link_libraries(molartracker_repo
    PUBLIC
    molartracker_finance # TODO: remove this form here
    PRIVATE
    molartracker_orm
    molartracker_db
    molartracker_logging
    molartracker_settings
    molartracker_config
    molartracker_sql_models
    molartracker_drafts
    molartracker_domain
)
