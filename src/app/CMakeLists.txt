add_library(molartracker_app STATIC
    src/app/repo_container.cpp
    src/app/service_container.cpp
    src/app/store_container.cpp
    src/app/app_context.cpp

    src/app/services/account_service.cpp
    src/app/services/profile_service.cpp

    src/app/repos/account_repo.cpp
    src/app/repos/profile_repo.cpp
    src/app/repos/repo_errors.cpp

    src/app/store/account_store.cpp
    src/app/store/profile/profile_store.cpp
    src/app/store/profile/active_profile.cpp
    src/app/store/profile/exception.cpp
    src/app/store/profile/predicates.cpp

    src/app/factories/account_factory.cpp
    src/app/factories/profile_factory.cpp

    src/app/domain/profile.cpp

    src/app/migration/migration_runner.cpp
    src/app/migration/migration.cpp
    src/app/migration/single_migration.cpp
)

target_include_directories(molartracker_app
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include/
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/
)

target_link_libraries(molartracker_app
    PUBLIC
    molartracker_finance
    molartracker_connections
    molartracker_orm # needs to be public because of singlemigration templates! TODO: resolve this
    PRIVATE
    molartracker_db
    molartracker_logging
    molartracker_settings
    molartracker_config
    molartracker_sql_models
    molartracker_drafts
)
