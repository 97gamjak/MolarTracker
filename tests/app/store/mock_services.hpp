#ifndef __TESTS__APP__STORE__MOCK_SERVICES_HPP__
#define __TESTS__APP__STORE__MOCK_SERVICES_HPP__

#include <algorithm>
#include <optional>
#include <set>
#include <string>
#include <vector>

#include "config/id_types.hpp"
#include "domain/profile.hpp"
#include "finance/account.hpp"
#include "finance/instrument/stock.hpp"
#include "finance/position.hpp"
#include "finance/transaction/domain_transaction.hpp"
#include "finance/transaction/transaction_filter.hpp"
#include "service/i_account_service.hpp"
#include "service/i_instrument_service.hpp"
#include "service/i_position_service.hpp"
#include "service/i_profile_service.hpp"
#include "service/i_transaction_service.hpp"

namespace tests
{

    class MockProfileService : public service::IProfileService
    {
       public:
        // NOLINTBEGIN(misc-non-private-member-variables-in-classes)
        std::vector<domain::Profile> profiles;
        int                          createCallCount = 0;
        int                          removeCallCount = 0;
        int                          updateCallCount = 0;
        // NOLINTEND(misc-non-private-member-variables-in-classes)

       private:
        int _nextId = 1;

       public:
        void addTestProfile(
            const std::string&                name,
            const std::optional<std::string>& email = std::nullopt
        )
        {
            auto profile = domain::Profile{ProfileId::invalid(), name, email};
            profile.setId(ProfileId{_nextId++});
            profiles.push_back(profile);
        }

        [[nodiscard]] std::vector<domain::Profile> getAll() const override
        {
            return profiles;
        }

        [[nodiscard]] std::optional<domain::Profile> get(
            ProfileId id
        ) const override
        {
            for (const auto& profile : profiles)
            {
                if (profile.getId() == id)
                    return profile;
            }
            return std::nullopt;
        }

        [[nodiscard]] ProfileId create(const domain::Profile& profile) override
        {
            createCallCount++;
            auto       newProfile = profile;
            const auto newId      = ProfileId{_nextId++};
            newProfile.setId(newId);
            profiles.push_back(newProfile);
            return newId;
        }

        void update(
            ProfileId                         id,
            const std::string&                newName,
            const std::optional<std::string>& newEmail
        ) override
        {
            updateCallCount++;
            for (auto& profile : profiles)
            {
                if (profile.getId() == id)
                {
                    profile.setName(newName);
                    profile.setEmail(newEmail);
                    return;
                }
            }
        }

        void remove(ProfileId id) override
        {
            removeCallCount++;
            const auto it = std::ranges::find_if(
                profiles,
                [id](const auto& profile) { return profile.getId() == id; }
            );

            if (it != profiles.end())
                profiles.erase(it);
        }
    };

    class MockAccountService : public service::IAccountService
    {
       public:
        // NOLINTBEGIN(misc-non-private-member-variables-in-classes)
        std::vector<finance::Account> preloadedAccounts;
        int                           createCallCount = 0;
        // NOLINTEND(misc-non-private-member-variables-in-classes)

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

    class MockInstrumentService : public service::IInstrumentService
    {
       public:
        // NOLINTBEGIN(misc-non-private-member-variables-in-classes)
        std::set<std::string> stocksInDb;
        int                   addStockCallCount = 0;
        // NOLINTEND(misc-non-private-member-variables-in-classes)

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
                .stockId      = StockId{_nextStockId++},
                .instrumentId = InstrumentId{_nextInstrumentId++}
            };
        }

        [[nodiscard]] bool stockExists(const std::string& ticker) override
        {
            return stocksInDb.contains(ticker);
        }
    };

    class MockPositionService : public service::IPositionService
    {
       public:
        // NOLINTBEGIN(misc-non-private-member-variables-in-classes)
        int createCallCount = 0;
        // NOLINTEND(misc-non-private-member-variables-in-classes)

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

    class MockTransactionService : public service::ITransactionService
    {
       public:
        // NOLINTBEGIN(misc-non-private-member-variables-in-classes)
        int addCallCount = 0;
        // NOLINTEND(misc-non-private-member-variables-in-classes)

       private:
        int _nextId = 1;

       public:
        [[nodiscard]] TransactionId addTransaction(
            const finance::DomainTransaction& /*transaction*/
        ) override
        {
            addCallCount++;
            return TransactionId{_nextId++};
        }

        [[nodiscard]] std::vector<finance::DomainTransaction> getTransactions(
            const idSet<AccountId>& /*accountIds*/,
            const finance::TransactionFilter& /*filter*/
        ) override
        {
            return {};
        }
    };

}   // namespace tests

#endif   // __TESTS__APP__STORE__MOCK_SERVICES_HPP__
