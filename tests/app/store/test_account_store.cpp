#include <gtest/gtest.h>

#include <memory>
#include <optional>
#include <vector>

#include "config/finance.hpp"
#include "config/id_types.hpp"
#include "drafts/account_draft.hpp"
#include "finance/account.hpp"
#include "mock_services.hpp"
#include "store/account/account_store.hpp"

namespace
{

    class AccountStoreTest : public ::testing::Test
    {
       protected:
        // NOLINTBEGIN(misc-non-private-member-variables-in-classes)
        std::shared_ptr<tests::MockAccountService> _mockService;
        std::unique_ptr<store::AccountStore>       _store;
        // NOLINTEND(misc-non-private-member-variables-in-classes)

        AccountStoreTest()
            : _mockService{std::make_shared<tests::MockAccountService>()},
              _store{std::make_unique<store::AccountStore>(_mockService)}
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

        void setActiveProfile() { _store->updateActiveProfile(ProfileId{1}); }
    };

}   // namespace

TEST_F(AccountStoreTest, CreateAccountNoActiveProfileReturnsError)
{
    const auto result = _store->createAccount(makeDraft("Savings"));

    EXPECT_EQ(result, store::AccountStoreResult::Error);
}

TEST_F(AccountStoreTest, CreateAccountWithActiveProfileReturnsOk)
{
    setActiveProfile();

    const auto result = _store->createAccount(makeDraft("Savings"));

    EXPECT_EQ(result, store::AccountStoreResult::Ok);
}

TEST_F(AccountStoreTest, CreateAccountDuplicateNameAndKindReturnsConflict)
{
    setActiveProfile();
    static_cast<void>(_store->createAccount(makeDraft("Savings")));

    const auto result = _store->createAccount(makeDraft("Savings"));

    EXPECT_EQ(result, store::AccountStoreResult::AccountNameConflict);
}

TEST_F(AccountStoreTest, GetAllAccountsEmptyInitially)
{
    const auto accounts = _store->getAllAccounts();

    EXPECT_TRUE(accounts.empty());
}

TEST_F(AccountStoreTest, GetAllAccountsReturnsCreatedAccount)
{
    setActiveProfile();
    static_cast<void>(_store->createAccount(makeDraft("Savings")));

    const auto accounts = _store->getAllAccounts();

    ASSERT_EQ(accounts.size(), 1U);
    EXPECT_EQ(accounts[0].name, "Savings");
}

TEST_F(AccountStoreTest, GetCashAccountsReturnsOnlyCashAccounts)
{
    setActiveProfile();
    static_cast<void>(
        _store->createAccount(makeDraft("CashAcc", AccountKind::Cash))
    );
    static_cast<void>(
        _store->createAccount(makeDraft("SecAcc", AccountKind::Security))
    );

    const auto cashAccounts = _store->getCashAccounts();

    ASSERT_EQ(cashAccounts.size(), 1U);
    EXPECT_EQ(cashAccounts[0].name, "CashAcc");
}

TEST_F(AccountStoreTest, GetSecurityAccountsReturnsOnlySecurityAccounts)
{
    setActiveProfile();
    static_cast<void>(
        _store->createAccount(makeDraft("CashAcc", AccountKind::Cash))
    );
    static_cast<void>(
        _store->createAccount(makeDraft("SecAcc", AccountKind::Security))
    );

    const auto secAccounts = _store->getSecurityAccounts();

    ASSERT_EQ(secAccounts.size(), 1U);
    EXPECT_EQ(secAccounts[0].name, "SecAcc");
}

TEST_F(AccountStoreTest, UpdateActiveProfileLoadsAccountsFromService)
{
    _mockService->preloadedAccounts.emplace_back(
        AccountStatus::Active,
        "LoadedAcc",
        Currency::EUR,
        AccountKind::Security

    );

    setActiveProfile();

    const auto accounts = _store->getAllAccounts();
    ASSERT_EQ(accounts.size(), 1U);
    EXPECT_EQ(accounts[0].name, "LoadedAcc");
}

TEST_F(AccountStoreTest, UpdateActiveProfileNulloptClearsActiveProfile)
{
    setActiveProfile();
    static_cast<void>(_store->createAccount(makeDraft("Savings")));

    _store->updateActiveProfile(std::nullopt);

    EXPECT_TRUE(_store->getAllAccounts().empty());
}

TEST_F(AccountStoreTest, CommitNewAccountCallsService)
{
    setActiveProfile();
    static_cast<void>(_store->createAccount(makeDraft("Savings")));

    _store->commit();

    EXPECT_GE(_mockService->createCallCount, 1);
}

TEST_F(AccountStoreTest, CreateCashAccountAlsoCreatesExternalAccount)
{
    setActiveProfile();
    static_cast<void>(_store->createAccount(
        makeDraft("CashAcc", AccountKind::Cash, Currency::EUR)
    ));

    EXPECT_NO_THROW(
        static_cast<void>(_store->getExternalAccount(Currency::EUR))
    );
}

TEST_F(AccountStoreTest, IsDirtyFalseInitially)
{
    EXPECT_FALSE(_store->isDirty());
}

TEST_F(AccountStoreTest, IsDirtyTrueAfterCreateAccount)
{
    setActiveProfile();
    static_cast<void>(_store->createAccount(makeDraft("Savings")));

    EXPECT_TRUE(_store->isDirty());
}
