#include "account_factory.hpp"

#include "config/id_types.hpp"
#include "finance/account.hpp"
#include "sql_models/account_row.hpp"

namespace app
{

    /**
     * @brief Convert an AccountRow database model to a
     * CashAccount
     *
     * @param accountRow
     * @return finance::CashAccount
     */
    finance::Account AccountFactory::toAccountDomain(
        const AccountRow& accountRow
    )
    {
        return {
            accountRow.id.value(),
            accountRow.status.value(),
            accountRow.name.value(),
            accountRow.currency.value(),
            accountRow.kind.value()
        };
    }

    /**
     * @brief Convert a vector of AccountRow database models to a vector of
     * Account domain models
     *
     * @param accountRows
     * @return std::vector<finance::Account>
     */
    std::vector<finance::Account> AccountFactory::toAccountDomains(
        const std::vector<AccountRow>& accountRows
    )
    {
        std::vector<finance::Account> accounts;
        accounts.reserve(accountRows.size());

        for (const auto& row : accountRows)
            accounts.push_back(toAccountDomain(row));

        return accounts;
    }

    /**
     * @brief Convert an account domain model to a database row
     *
     * @param account
     * @param profileId
     * @return AccountRow
     */
    AccountRow AccountFactory::toAccountRow(
        const finance::Account& account,
        const ProfileId&        profileId
    )
    {
        AccountRow accountRow;

        accountRow.id        = account.getId();
        accountRow.kind      = account.getKind();
        accountRow.status    = account.getStatus();
        accountRow.name      = account.getName();
        accountRow.currency  = account.getCurrency();
        accountRow.profileId = profileId;

        return accountRow;
    }

}   // namespace app