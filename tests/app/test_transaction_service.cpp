#include <gtest/gtest.h>

#include <cstdint>
#include <memory>
#include <vector>

#include "common/cash.hpp"
#include "common/finance.hpp"
#include "common/quantity.hpp"
#include "common/timestamp.hpp"
#include "config/id_types.hpp"
#include "db/database.hpp"
#include "finance/transaction/domain_transaction.hpp"
#include "finance/transaction/transaction_entries.hpp"
#include "finance/transaction/transaction_entry.hpp"
#include "finance/transaction/transaction_filter.hpp"
#include "repo/migration/migration_runner.hpp"
#include "repo/transaction_repo.hpp"
#include "service/transaction_service.hpp"
#include "test_fixtures.hpp"

namespace
{

    constexpr std::int64_t TEST_TS = 1'715'000'000'000LL;

    class TransactionServiceTest : public ::testing::Test
    {
       protected:
        // NOLINTBEGIN(misc-non-private-member-variables-in-classes)
        const AccountId _accountId{1};

        tests::TempDbFile                            _tempFile;
        db::Database                                 _db;
        std::shared_ptr<repo::TransactionRepo>       _repo;
        std::shared_ptr<service::TransactionService> _service;
        // NOLINTEND(misc-non-private-member-variables-in-classes)

        TransactionServiceTest()
            : _db{_tempFile.path()},
              _repo{std::make_shared<repo::TransactionRepo>(_db)},
              _service{std::make_shared<service::TransactionService>(_repo)}
        {
            auto runner = repo::MigrationRunner{_db};
            static_cast<void>(_db.execute(
                "INSERT INTO profile (name, email) "
                "VALUES ('TestProfile', NULL)"
            ));
            static_cast<void>(_db.execute(
                "INSERT INTO account "
                "(kind, profile_id, name, status, currency) "
                "VALUES (0, 1, 'TestAccount', 0, 0)"
            ));
        }

        [[nodiscard]] finance::DomainTransaction makeCashTx(
            micro_units amount = 0
        ) const
        {
            const auto entry = finance::TransactionEntry{
                TransactionEntryId::invalid(),
                _accountId,
                Cash{Currency::USD, amount},
                TransactionEntryType::General
            };
            return finance::DomainTransaction{
                TransactionId::invalid(),
                Timestamp::fromInt64(TEST_TS),
                TransactionStatus::Completed,
                finance::CashData{},
                finance::TransactionEntries{{entry}},
                std::nullopt
            };
        }
    };

}   // namespace

TEST_F(TransactionServiceTest, AddTransactionReturnsValidId)
{
    const auto id = _service->addTransaction(makeCashTx());

    EXPECT_GT(id.value(), 0);
}

TEST_F(TransactionServiceTest, GetTransactionsEmptyForEmptyAccountSet)
{
    const auto txs = _service->getTransactions(finance::TransactionFilter{});

    EXPECT_TRUE(txs.empty());
}

TEST_F(TransactionServiceTest, GetTransactionsReturnsAddedTransaction)
{
    static_cast<void>(_service->addTransaction(makeCashTx()));

    finance::TransactionFilter filter;
    filter.accountIds.insert(_accountId);

    const auto txs = _service->getTransactions(filter);

    EXPECT_EQ(txs.size(), 1U);
}

TEST_F(TransactionServiceTest, AddMultipleTransactionsIdsAreDistinct)
{
    const auto id1 = _service->addTransaction(makeCashTx());
    const auto id2 = _service->addTransaction(makeCashTx());

    EXPECT_NE(id1.value(), id2.value());
}
