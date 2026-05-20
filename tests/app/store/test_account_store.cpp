#include <gtest/gtest.h>

#include <memory>
#include <optional>
#include <vector>

#include "app/store/account/account_store.hpp"
#include "config/finance.hpp"
#include "config/id_types.hpp"
#include "drafts/account_draft.hpp"
#include "finance/account.hpp"
#include "mock_services.hpp"

namespace
{

    class AccountStoreTest : public ::testing::Test
    {
       protected:
        // NOLINTBEGIN(misc-non-private-member-variables-in-classes)
        std::shared_ptr<tests::MockAccountService> _mockService;
        std::unique_ptr<app::AccountStore>         _store;
        // NOLINTEND(misc-non-private-member-variables-in-classes)

        AccountStoreTest()
            : _mockService{std::make_shared<tests::MockAccountService>()},
              _store{std::make_unique<app::AccountStore>(_mockService)}
        {
        }

        [[nodiscard]] static drafts::AccountDraft makeDraft(
            const std::string& name,
            AccountKind        kind     = AccountKind::Security,
            Currency           currency = Currency::EUR
        )
        {
            return drafts::AccountDraft{
                AccountId::invalid(),
                name,
                kind,
                currency,
                std::nullopt
            };
        }

        void setActiveProfile()
        {
            _store->updateActiveProfile(ProfileId{1});
        }
    };

}   // namespace

TEST_F(AccountStoreTest, CreateAccount_NoActiveProfile_ReturnsError)
{
    const auto result = _store->createAccount(makeDraft("Savings"));

    EXPECT_EQ(result, app::AccountStoreResult::Error);
}

TEST_F(AccountStoreTest, CreateAccount_WithActiveProfile_ReturnsOk)
{
    setActiveProfile();

    const auto result = _store->createAccount(makeDraft("Savings"));

    EXPECT_EQ(result, app::AccountStoreResult::Ok);
}

TEST_F(AccountStoreTest, CreateAccount_DuplicateNameAndKind_ReturnsConflict)
{
    setActiveProfile();
    static_cast<void>(_store->createAccount(makeDraft("Savings")));

    const auto result = _store->createAccount(makeDraft("Savings"));

    EXPECT_EQ(result, app::AccountStoreResult::AccountNameConflict);
}

TEST_F(AccountStoreTest, GetAllAccounts_EmptyInitially)
{
    const auto accounts = _store->getAllAccounts();

    EXPECT_TRUE(accounts.empty());
}

TEST_F(AccountStoreTest, GetAllAccounts_ReturnsCreatedAccount)
{
    setActiveProfile();
    static_cast<void>(_store->createAccount(makeDraft("Savings")));

    const auto accounts = _store->getAllAccounts();

    ASSERT_EQ(accounts.size(), 1u);
    EXPECT_EQ(accounts[0].name, "Savings");
}

TEST_F(AccountStoreTest, GetCashAccounts_ReturnsOnlyCashAccounts)
{
    setActiveProfile();
    static_cast<void>(
        _store->createAccount(makeDraft("CashAcc", AccountKind::Cash))
    );
    static_cast<void>(
        _store->createAccount(makeDraft("SecAcc", AccountKind::Security))
    );

    const auto cashAccounts = _store->getCashAccounts();

    ASSERT_EQ(cashAccounts.size(), 1u);
    EXPECT_EQ(cashAccounts[0].name, "CashAcc");
}

TEST_F(AccountStoreTest, GetSecurityAccounts_ReturnsOnlySecurityAccounts)
{
    setActiveProfile();
    static_cast<void>(
        _store->createAccount(makeDraft("CashAcc", AccountKind::Cash))
    );
    static_cast<void>(
        _store->createAccount(makeDraft("SecAcc", AccountKind::Security))
    );

    const auto secAccounts = _store->getSecurityAccounts();

    ASSERT_EQ(secAccounts.size(), 1u);
    EXPECT_EQ(secAccounts[0].name, "SecAcc");
}

TEST_F(AccountStoreTest, UpdateActiveProfile_LoadsAccountsFromService)
{
    _mockService->preloadedAccounts.push_back(
        finance::Account{
            AccountStatus::Active,
            "LoadedAcc",
            Currency::EUR,
            AccountKind::Security
        }
    );

    setActiveProfile();

    const auto accounts = _store->getAllAccounts();
    ASSERT_EQ(accounts.size(), 1u);
    EXPECT_EQ(accounts[0].name, "LoadedAcc");
}

TEST_F(AccountStoreTest, UpdateActiveProfile_NulloptClearsActiveProfile)
{
    setActiveProfile();
    static_cast<void>(_store->createAccount(makeDraft("Savings")));

    _store->updateActiveProfile(std::nullopt);

    EXPECT_TRUE(_store->getAllAccounts().empty());
}

TEST_F(AccountStoreTest, Commit_NewAccount_CallsService)
{
    setActiveProfile();
    static_cast<void>(_store->createAccount(makeDraft("Savings")));

    _store->commit();

    EXPECT_GE(_mockService->createCallCount, 1);
}

TEST_F(AccountStoreTest, CreateCashAccount_AlsoCreatesExternalAccount)
{
    setActiveProfile();
    static_cast<void>(
        _store->createAccount(
            makeDraft("CashAcc", AccountKind::Cash, Currency::EUR)
        )
    );

    EXPECT_NO_THROW(
        static_cast<void>(_store->getExternalAccount(Currency::EUR))
    );
}

TEST_F(AccountStoreTest, IsDirty_FalseInitially)
{
    EXPECT_FALSE(_store->isDirty());
}

TEST_F(AccountStoreTest, IsDirty_TrueAfterCreateAccount)
{
    setActiveProfile();
    static_cast<void>(_store->createAccount(makeDraft("Savings")));

    EXPECT_TRUE(_store->isDirty());
}
