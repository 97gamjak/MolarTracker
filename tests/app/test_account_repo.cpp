#include <gtest/gtest.h>

#include <cassert>
#include <filesystem>
#include <random>
#include <string>
#include <vector>

#include "app/migration/migration_runner.hpp"
#include "app/repos/account_repo.hpp"
#include "config/finance.hpp"
#include "config/id_types.hpp"
#include "db/database.hpp"
#include "finance/account.hpp"
#include "orm/crud.hpp"
#include "orm/crud/crud_error.hpp"
#include "sql_models/profile_row.hpp"

namespace
{

    std::filesystem::path unique_db_path()
    {
        namespace fs = std::filesystem;

        fs::path base;
        try
        {
            base = fs::temp_directory_path();
        }
        catch (...)
        {
            base = fs::current_path();
        }

        std::random_device random;
        const auto         random1 = static_cast<unsigned>(random());
        const auto         random2 = static_cast<unsigned>(random());

        return base /
               ("molartracker_account_repo_test_" + std::to_string(random1) +
                "_" + std::to_string(random2) + ".sqlite");
    }

    class TempDbFile
    {
       public:
        TempDbFile() : _path(unique_db_path()) {}

        TempDbFile(const TempDbFile&)            = delete;
        TempDbFile& operator=(const TempDbFile&) = delete;
        TempDbFile(TempDbFile&&)                 = default;
        TempDbFile& operator=(TempDbFile&&)      = default;

        ~TempDbFile()
        {
            std::error_code errorCode;
            std::filesystem::remove(_path, errorCode);
        }

        [[nodiscard]] const std::filesystem::path& path() const noexcept
        {
            return _path;
        }

       private:
        std::filesystem::path _path;
    };

    class AccountRepoTest : public ::testing::Test
    {
       protected:
        TempDbFile       _tempFile;
        db::Database     _db;
        app::AccountRepo _repo;

        AccountRepoTest() : _db{_tempFile.path()}, _repo{_db}
        {
            app::MigrationRunner{_db};
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
            return finance::Account{status, name, currency, kind};
        }
    };

}   // namespace

TEST_F(AccountRepoTest, CreateAccount_ReturnsValidId)
{
    const auto profileId = insertProfile("User");
    const auto account   = makeAccount("Savings");

    const auto id = _repo.createAccount(account, profileId);

    EXPECT_GT(id.value(), 0);
}

TEST_F(AccountRepoTest, CreateAccount_PersistsAccountInDatabase)
{
    const auto profileId = insertProfile("User");

    const auto returnedId =
        _repo.createAccount(makeAccount("Checking"), profileId);

    const auto accounts = _repo.getAllAccounts(profileId);
    ASSERT_EQ(accounts.size(), 1u);
    EXPECT_EQ(accounts[0].getId(), returnedId);
    EXPECT_EQ(accounts[0].getName(), "Checking");
}

TEST_F(AccountRepoTest, CreateAccount_DuplicateUniqueKeyThrows)
{
    const auto profileId = insertProfile("User");
    const auto account   = makeAccount("Savings");

    static_cast<void>(_repo.createAccount(account, profileId));

    EXPECT_THROW(
        const auto result = _repo.createAccount(account, profileId),
        orm::CrudException
    );
}

TEST_F(AccountRepoTest, CreateAccount_SameNameDifferentKindSucceeds)
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

TEST_F(AccountRepoTest, CreateAccount_SameKindAndNameDifferentProfileSucceeds)
{
    const auto profileId1 = insertProfile("User1");
    const auto profileId2 = insertProfile("User2");
    const auto account    = makeAccount("SharedName");

    static_cast<void>(_repo.createAccount(account, profileId1));

    EXPECT_NO_THROW(
        const auto result = _repo.createAccount(account, profileId2)
    );
}

TEST_F(AccountRepoTest, GetAllAccounts_EmptyWhenNoAccounts)
{
    const auto profileId = insertProfile("User");

    const auto accounts = _repo.getAllAccounts(profileId);

    EXPECT_TRUE(accounts.empty());
}

TEST_F(AccountRepoTest, GetAllAccounts_ReturnsAllAccountsForProfile)
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

    EXPECT_EQ(accounts.size(), 3u);
}

TEST_F(AccountRepoTest, GetAllAccounts_IsolatesAccountsByProfile)
{
    const auto profileId1 = insertProfile("User1");
    const auto profileId2 = insertProfile("User2");

    static_cast<void>(_repo.createAccount(makeAccount("P1Account"), profileId1)
    );
    static_cast<void>(_repo.createAccount(makeAccount("P2Account"), profileId2)
    );

    const auto accounts1 = _repo.getAllAccounts(profileId1);
    const auto accounts2 = _repo.getAllAccounts(profileId2);

    ASSERT_EQ(accounts1.size(), 1u);
    ASSERT_EQ(accounts2.size(), 1u);
    EXPECT_EQ(accounts1[0].getName(), "P1Account");
    EXPECT_EQ(accounts2[0].getName(), "P2Account");
}

TEST_F(AccountRepoTest, GetAllAccounts_ReturnsCorrectAccountData)
{
    const auto profileId = insertProfile("User");
    const auto account   = finance::Account{
        AccountStatus::Active,
        "MyWallet",
        Currency::CHF,
        AccountKind::Cash
    };

    static_cast<void>(_repo.createAccount(account, profileId));

    const auto accounts = _repo.getAllAccounts(profileId);
    ASSERT_EQ(accounts.size(), 1u);

    const auto& retrieved = accounts[0];
    EXPECT_EQ(retrieved.getName(), "MyWallet");
    EXPECT_EQ(retrieved.getCurrency(), Currency::CHF);
    EXPECT_EQ(retrieved.getKind(), AccountKind::Cash);
    EXPECT_EQ(retrieved.getStatus(), AccountStatus::Active);
    EXPECT_GT(retrieved.getId().value(), 0);
}
