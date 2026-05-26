#include <gtest/gtest.h>

#include <cassert>
#include <memory>
#include <string>
#include <vector>

#include "config/finance.hpp"
#include "config/id_types.hpp"
#include "db/database.hpp"
#include "domain/account.hpp"
#include "orm/crud.hpp"
#include "orm/crud/crud_error.hpp"
#include "repo/account_repo.hpp"
#include "repo/migration/migration_runner.hpp"
#include "service/account_service.hpp"
#include "sql_models/profile_row.hpp"
#include "test_fixtures.hpp"

namespace
{

    class AccountServiceTest : public ::testing::Test
    {
       protected:
        // NOLINTBEGIN(misc-non-private-member-variables-in-classes)
        tests::TempDbFile                        _tempFile;
        db::Database                             _db;
        std::shared_ptr<repo::AccountRepo>       _repo;
        std::shared_ptr<service::AccountService> _service;
        // NOLINTEND(misc-non-private-member-variables-in-classes)

        AccountServiceTest()
            : _db{_tempFile.path()},
              _repo{std::make_shared<repo::AccountRepo>(_db)},
              _service{std::make_shared<service::AccountService>(_repo)}
        {
            repo::MigrationRunner{_db};
        }

        [[nodiscard]] ProfileId insertProfile(const std::string& name)
        {
            ProfileRow row;
            row.name    = name;
            auto result = orm::Crud().insert(_db, row);
            assert(result.has_value());
            return ProfileId(result.value());
        }

        [[nodiscard]] static domain::Account makeAccount(
            const std::string& name,
            AccountKind        kind     = AccountKind::Cash,
            Currency           currency = Currency::EUR
        )
        {
            return domain::Account{
                AccountId::invalid(),
                AccountStatus::Active,
                name,
                currency,
                kind
            };
        }
    };

}   // namespace

TEST_F(AccountServiceTest, CreateAccountReturnsValidId)
{
    const auto profileId = insertProfile("User");
    const auto account   = makeAccount("Savings");

    const auto id = _service->createAccount(account, profileId);

    EXPECT_GT(id.value(), 0);
}

TEST_F(AccountServiceTest, GetAllAccountsEmptyForNewProfile)
{
    const auto profileId = insertProfile("User");

    const auto accounts = _service->getAllAccounts(profileId);

    EXPECT_TRUE(accounts.empty());
}

TEST_F(AccountServiceTest, GetAllAccountsReturnsCreatedAccount)
{
    const auto profileId = insertProfile("User");

    static_cast<void>(
        _service->createAccount(makeAccount("Checking"), profileId)
    );

    const auto accounts = _service->getAllAccounts(profileId);

    ASSERT_EQ(accounts.size(), 1U);
    EXPECT_EQ(accounts[0].getName(), "Checking");
}

TEST_F(AccountServiceTest, GetAllAccountsIsolatedByProfile)
{
    const auto profileId1 = insertProfile("Alice");
    const auto profileId2 = insertProfile("Bob");

    static_cast<void>(
        _service->createAccount(makeAccount("AliceAcc"), profileId1)
    );

    const auto accounts = _service->getAllAccounts(profileId2);

    EXPECT_TRUE(accounts.empty());
}

TEST_F(AccountServiceTest, CreateAccountDuplicateNameAndKindThrows)
{
    const auto profileId = insertProfile("User");

    static_cast<void>(_service->createAccount(makeAccount("Savings"), profileId)
    );

    EXPECT_THROW(
        static_cast<void>(
            _service->createAccount(makeAccount("Savings"), profileId)
        ),
        orm::CrudException
    );
}
