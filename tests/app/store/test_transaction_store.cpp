#include <gtest/gtest.h>

#include <cstdint>
#include <memory>
#include <vector>

#include "config/finance.hpp"
#include "config/id_types.hpp"
#include "config/quantity.hpp"
#include "finance/cash.hpp"
#include "finance/transaction/domain_transaction.hpp"
#include "finance/transaction/transaction_entry.hpp"
#include "finance/transaction/transaction_filter.hpp"
#include "mock_services.hpp"
#include "store/account/account_session.hpp"
#include "store/account/account_store.hpp"
#include "store/position_store.hpp"
#include "store/transaction_store.hpp"
#include "utils/timestamp.hpp"

namespace
{

    constexpr std::int64_t TEST_TS = 1'715'000'000'000LL;

    class TransactionStoreTest : public ::testing::Test
    {
       protected:
        // NOLINTBEGIN(misc-non-private-member-variables-in-classes)
        std::shared_ptr<tests::MockAccountService>     _mockAccountService;
        std::shared_ptr<tests::MockInstrumentService>  _mockInstrumentService;
        std::shared_ptr<tests::MockPositionService>    _mockPositionService;
        std::shared_ptr<tests::MockTransactionService> _mockTransactionService;
        InstrumentIdSeq                                _idSeq;
        store::AccountStore                            _accountStore;
        store::AccountSession                          _accountSession;
        store::PositionStore                           _positionStore;
        std::unique_ptr<store::TransactionStore>       _store;
        // NOLINTEND(misc-non-private-member-variables-in-classes)

        TransactionStoreTest()
            : _mockAccountService{std::make_shared<tests::MockAccountService>(
              )},
              _mockInstrumentService{
                  std::make_shared<tests::MockInstrumentService>()
              },
              _mockPositionService{std::make_shared<tests::MockPositionService>(
              )},
              _mockTransactionService{
                  std::make_shared<tests::MockTransactionService>()
              },
              _accountStore{_mockAccountService},
              _positionStore{_mockPositionService, _accountSession},
              _store{std::make_unique<store::TransactionStore>(
                  _mockTransactionService,
                  _accountSession
              )}
        {
        }

        /// Returns a cash transaction whose entries sum to zero (empty).
        [[nodiscard]] static finance::DomainTransaction makeZeroSumTx()
        {
            return finance::DomainTransaction{
                TransactionId::invalid(),
                Timestamp::fromInt64(TEST_TS),
                TransactionStatus::Completed,
                finance::CashData{},
                {},
                std::nullopt
            };
        }

        /// Returns a cash transaction whose single entry has a non-zero amount.
        [[nodiscard]] static finance::DomainTransaction makeNonZeroSumTx()
        {
            const auto cash = 100'000;
            return finance::DomainTransaction{
                TransactionId::invalid(),
                Timestamp::fromInt64(TEST_TS),
                TransactionStatus::Completed,
                finance::CashData{},
                finance::TransactionEntries{{finance::TransactionEntry{
                    TransactionEntryId::invalid(),
                    AccountId{1},
                    finance::Cash{Currency::USD, micro_units{cash}},
                    TransactionEntryType::General
                }}},
                std::nullopt
            };
        }
    };

}   // namespace

TEST_F(TransactionStoreTest, AddTransactionZeroSumReturnsOk)
{
    const auto result = _store->addTransaction(makeZeroSumTx());

    EXPECT_EQ(result, store::TransactionStoreResult::Ok);
}

TEST_F(TransactionStoreTest, AddTransactionNonZeroSumReturnsError)
{
    const auto result = _store->addTransaction(makeNonZeroSumTx());

    EXPECT_EQ(result, store::TransactionStoreResult::TransactionSumNotZero);
}

TEST_F(TransactionStoreTest, GetTransactionsEmptyWhenNoAccounts)
{
    static_cast<void>(_store->addTransaction(makeZeroSumTx()));

    const auto txs = _store->getTransactions(finance::TransactionFilter{});

    EXPECT_TRUE(txs.empty());
}

TEST_F(TransactionStoreTest, IsDirtyFalseInitially)
{
    EXPECT_FALSE(_store->isDirty());
}

TEST_F(TransactionStoreTest, IsDirtyTrueAfterAddTransaction)
{
    static_cast<void>(_store->addTransaction(makeZeroSumTx()));

    EXPECT_TRUE(_store->isDirty());
}

TEST_F(TransactionStoreTest, CommitNewTransactionCallsService)
{
    static_cast<void>(_store->addTransaction(makeZeroSumTx()));

    _store->commit({}, {}, {});

    EXPECT_EQ(_mockTransactionService->addCallCount, 1);
}

TEST_F(TransactionStoreTest, CommitMultipleTransactionsCallsServiceForEach)
{
    static_cast<void>(_store->addTransaction(makeZeroSumTx()));
    static_cast<void>(_store->addTransaction(makeZeroSumTx()));

    _store->commit({}, {}, {});

    EXPECT_EQ(_mockTransactionService->addCallCount, 2);
}
