#include <gtest/gtest.h>

#include <cassert>
#include <string>
#include <vector>

#include "common/finance.hpp"
#include "config/id_types.hpp"
#include "db/database.hpp"
#include "finance/account/account.hpp"
#include "orm/crud.hpp"
#include "orm/crud/crud_error.hpp"
#include "repo/account_repo.hpp"
#include "repo/migration/migration_runner.hpp"
#include "sql_models/profile_row.hpp"
#include "test_fixtures.hpp"

namespace
{

    class AccountRepoTest : public ::testing::Test
    {
       protected:
        // NOLINTBEGIN(misc-non-private-member-variables-in-classes)
        tests::TempDbFile _tempFile;
        db::Database      _db;
        repo::AccountRepo _repo;
        // NOLINTEND(misc-non-private-member-variables-in-classes)

        AccountRepoTest() : _db{_tempFile.path()}, _repo{_db}
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

        [[nodiscard]] static finance::Account makeAccount(
            const std::string& name,
            AccountKind        kind     = AccountKind::Cash,
            AccountStatus      status   = AccountStatus::Active,
            Currency           currency = Currency::EUR
        )
        {
            return finance::Account{
                AccountId::invalid(),
                status,
                name,
                currency,
                kind
            };
        }
    };

}   // namespace

TEST_F(AccountRepoTest, CreateAccountReturnsValidId)
{
    const auto profileId = insertProfile("User");
    const auto account   = makeAccount("Savings");

    const auto id = _repo.createAccount(account, profileId);

    EXPECT_GT(id.value(), 0);
}

TEST_F(AccountRepoTest, CreateAccountPersistsAccountInDatabase)
{
    const auto profileId = insertProfile("User");

    const auto returnedId =
        _repo.createAccount(makeAccount("Checking"), profileId);

    const auto accounts = _repo.getAllAccounts(profileId);
    ASSERT_EQ(accounts.size(), 1U);
    EXPECT_EQ(accounts[0].getId(), returnedId.value());
    EXPECT_EQ(accounts[0].getName(), "Checking");
}

TEST_F(AccountRepoTest, CreateAccountDuplicateUniqueKeyThrows)
{
    const auto profileId = insertProfile("User");
    const auto account   = makeAccount("Savings");

    static_cast<void>(_repo.createAccount(account, profileId));

    EXPECT_FALSE(_repo.createAccount(account, profileId));
}

TEST_F(AccountRepoTest, CreateAccountSameNameDifferentKindSucceeds)
{
    const auto profileId = insertProfile("User");

    static_cast<void>(_repo.createAccount(
        makeAccount("MyAccount", AccountKind::Cash),
        profileId
    ));

    EXPECT_NO_THROW(
        const auto result = _repo.createAccount(
            makeAccount("MyAccount", AccountKind::External),
            profileId
        )
    );
}

TEST_F(AccountRepoTest, CreateAccountSameKindAndNameDifferentProfileSucceeds)
{
    const auto profileId1 = insertProfile("User1");
    const auto profileId2 = insertProfile("User2");
    const auto account    = makeAccount("SharedName");

    static_cast<void>(_repo.createAccount(account, profileId1));

    EXPECT_NO_THROW(
        const auto result = _repo.createAccount(account, profileId2)
    );
}

TEST_F(AccountRepoTest, GetAllAccountsEmptyWhenNoAccounts)
{
    const auto profileId = insertProfile("User");

    const auto accounts = _repo.getAllAccounts(profileId);

    EXPECT_TRUE(accounts.empty());
}

TEST_F(AccountRepoTest, GetAllAccountsReturnsAllAccountsForProfile)
{
    const auto profileId = insertProfile("User");

    static_cast<void>(
        _repo.createAccount(makeAccount("Cash", AccountKind::Cash), profileId)
    );
    static_cast<void>(_repo.createAccount(
        makeAccount("Broker", AccountKind::Security),
        profileId
    ));
    static_cast<void>(_repo.createAccount(
        makeAccount("External", AccountKind::External),
        profileId
    ));

    const auto accounts = _repo.getAllAccounts(profileId);

    EXPECT_EQ(accounts.size(), 3U);
}

TEST_F(AccountRepoTest, GetAllAccountsIsolatesAccountsByProfile)
{
    const auto profileId1 = insertProfile("User1");
    const auto profileId2 = insertProfile("User2");

    static_cast<void>(_repo.createAccount(makeAccount("P1Account"), profileId1)
    );
    static_cast<void>(_repo.createAccount(makeAccount("P2Account"), profileId2)
    );

    const auto accounts1 = _repo.getAllAccounts(profileId1);
    const auto accounts2 = _repo.getAllAccounts(profileId2);

    ASSERT_EQ(accounts1.size(), 1U);
    ASSERT_EQ(accounts2.size(), 1U);
    EXPECT_EQ(accounts1[0].getName(), "P1Account");
    EXPECT_EQ(accounts2[0].getName(), "P2Account");
}

TEST_F(AccountRepoTest, GetAllAccountsReturnsCorrectAccountData)
{
    const auto profileId = insertProfile("User");
    const auto account   = finance::Account{
        AccountId::invalid(),
        AccountStatus::Active,
        "MyWallet",
        Currency::CHF,
        AccountKind::Cash
    };

    static_cast<void>(_repo.createAccount(account, profileId));

    const auto accounts = _repo.getAllAccounts(profileId);
    ASSERT_EQ(accounts.size(), 1U);

    const auto& retrieved = accounts[0];
    EXPECT_EQ(retrieved.getName(), "MyWallet");
    EXPECT_EQ(retrieved.getCurrency(), Currency::CHF);
    EXPECT_EQ(retrieved.getKind(), AccountKind::Cash);
    EXPECT_EQ(retrieved.getStatus(), AccountStatus::Active);
    EXPECT_GT(retrieved.getId().value(), 0);
}
