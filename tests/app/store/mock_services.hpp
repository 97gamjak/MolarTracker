#ifndef __TESTS__APP__STORE__MOCK_SERVICES_HPP__
#define __TESTS__APP__STORE__MOCK_SERVICES_HPP__

#include <algorithm>
#include <optional>
#include <set>
#include <string>
#include <vector>

#include "app/domain/profile.hpp"
#include "app/services_api/i_account_service.hpp"
#include "app/services_api/i_instrument_service.hpp"
#include "app/services_api/i_position_service.hpp"
#include "app/services_api/i_profile_service.hpp"
#include "app/services_api/i_transaction_service.hpp"
#include "config/id_types.hpp"
#include "finance/account.hpp"
#include "finance/instrument/stock.hpp"
#include "finance/position.hpp"
#include "finance/transaction.hpp"
#include "finance/transaction_filter.hpp"

namespace tests
{

    class MockProfileService : public app::IProfileService
    {
       public:
        std::vector<app::Profile> profiles;
        int                       createCallCount = 0;
        int                       removeCallCount = 0;
        int                       updateCallCount = 0;

       private:
        int _nextId = 1;

       public:
        void addTestProfile(
            const std::string&                name,
            const std::optional<std::string>& email = std::nullopt
        )
        {
            auto profile = app::Profile{name, email};
            profile.setId(ProfileId{_nextId++});
            profiles.push_back(profile);
        }

        [[nodiscard]] std::vector<app::Profile> getAll() const override
        {
            return profiles;
        }

        [[nodiscard]] std::optional<app::Profile> get(
            ProfileId id
        ) const override
        {
            for (const auto& p : profiles)
            {
                if (p.getId() == id)
                    return p;
            }
            return std::nullopt;
        }

        [[nodiscard]] ProfileId create(
            const std::string&                name,
            const std::optional<std::string>& email
        ) override
        {
            createCallCount++;
            auto profile = app::Profile{name, email};
            const auto newId = ProfileId{_nextId++};
            profile.setId(newId);
            profiles.push_back(profile);
            return newId;
        }

        void update(
            ProfileId                         id,
            const std::string&                newName,
            const std::optional<std::string>& newEmail
        ) override
        {
            updateCallCount++;
            for (auto& p : profiles)
            {
                if (p.getId() == id)
                {
                    p.setName(newName);
                    p.setEmail(newEmail);
                    return;
                }
            }
        }

        void remove(ProfileId id) override
        {
            removeCallCount++;
            profiles.erase(
                std::remove_if(
                    profiles.begin(),
                    profiles.end(),
                    [id](const auto& p) { return p.getId() == id; }
                ),
                profiles.end()
            );
        }
    };

    class MockAccountService : public app::IAccountService
    {
       public:
        std::vector<finance::Account> preloadedAccounts;
        int                           createCallCount = 0;

       private:
        int _nextId = 1;

       public:
        [[nodiscard]] std::vector<finance::Account> getAllAccounts(
            const ProfileId& /*profileId*/
        ) const override
        {
            return preloadedAccounts;
        }

        [[nodiscard]] AccountId createAccount(
            const finance::Account& /*account*/,
            const ProfileId& /*profileId*/
        ) override
        {
            createCallCount++;
            return AccountId{_nextId++};
        }
    };

    class MockInstrumentService : public app::IInstrumentService
    {
       public:
        std::set<std::string> stocksInDb;
        int                   addStockCallCount = 0;

       private:
        int _nextStockId      = 1;
        int _nextInstrumentId = 1;

       public:
        [[nodiscard]] std::vector<std::string> getTickers() override
        {
            return {};
        }

        [[nodiscard]] std::vector<finance::Stock> getStocks(
            const idSet<InstrumentId>& /*ids*/
        ) override
        {
            return {};
        }

        [[nodiscard]] std::optional<finance::Stock> getStock(
            const std::string& /*ticker*/
        ) override
        {
            return std::nullopt;
        }

        [[nodiscard]] finance::StockInsertionResult addStock(
            const finance::Stock& /*stock*/
        ) override
        {
            addStockCallCount++;
            return finance::StockInsertionResult{
                StockId{_nextStockId++},
                InstrumentId{_nextInstrumentId++}
            };
        }

        [[nodiscard]] bool stockExists(const std::string& ticker) override
        {
            return stocksInDb.count(ticker) > 0;
        }
    };

    class MockPositionService : public app::IPositionService
    {
       public:
        int createCallCount = 0;

       private:
        int _nextId = 1;

       public:
        [[nodiscard]] PositionId createPosition(
            const finance::Position& /*position*/
        ) override
        {
            createCallCount++;
            return PositionId{_nextId++};
        }

        [[nodiscard]] std::vector<finance::Position> getAllPositions(
            const idSet<AccountId>& /*accountIds*/
        ) override
        {
            return {};
        }

        [[nodiscard]] std::vector<finance::Position> getAllOpenPositions(
            const idSet<AccountId>& /*accountIds*/
        ) override
        {
            return {};
        }
    };

    class MockTransactionService : public app::ITransactionService
    {
       public:
        int addCallCount = 0;

       private:
        int _nextId = 1;

       public:
        [[nodiscard]] TransactionId addTransaction(
            const finance::Transaction& /*transaction*/
        ) override
        {
            addCallCount++;
            return TransactionId{_nextId++};
        }

        [[nodiscard]] std::vector<finance::Transaction> getTransactions(
            const idSet<AccountId>& /*accountIds*/,
            const finance::TransactionFilter& /*filter*/
        ) override
        {
            return {};
        }
    };

}   // namespace tests

#endif   // __TESTS__APP__STORE__MOCK_SERVICES_HPP__
