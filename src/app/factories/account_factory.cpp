#include "account_factory.hpp"

#include "config/id_types.hpp"
#include "finance/cash_account.hpp"
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
    finance::CashAccount AccountFactory::toCashAccountDomain(
        const AccountRow& accountRow
    )
    {
        return {
            accountRow.id.value(),
            accountRow.status.value(),
            accountRow.name.value(),
            accountRow.currency.value()
        };
    }

    /**
     * @brief Convert a vector of AccountRow database models to a vector of
     * CashAccount domain models
     *
     * @param accountRows
     * @return std::vector<finance::CashAccount>
     */
    std::vector<finance::CashAccount> AccountFactory::toCashAccountDomains(
        const std::vector<AccountRow>& accountRows
    )
    {
        std::vector<finance::CashAccount> accounts;
        accounts.reserve(accountRows.size());

        for (const auto& row : accountRows)
            accounts.push_back(toCashAccountDomain(row));

        return accounts;
    }

    /**
     * @brief Convert a CashAccount domain model to a pair of AccountRow and
     * CashAccountRow database models, this method takes a CashAccount object as
     * input and extracts the relevant information to populate an AccountRow and
     * a CashAccountRow, it returns a pair containing both rows.
     *
     * @param account
     * @param profileId
     * @return std::pair<AccountRow, CashAccountRow>
     */
    std::pair<AccountRow, CashAccountRow> AccountFactory::toCashAccountRow(
        const finance::CashAccount& account,
        const ProfileId&            profileId
    )
    {
        AccountRow     accountRow;
        CashAccountRow cashAccountRow;

        accountRow.id        = account.getId();
        accountRow.kind      = AccountKind::Cash;
        accountRow.status    = account.getStatus();
        accountRow.name      = account.getName();
        accountRow.currency  = account.getCurrency();
        accountRow.profileId = profileId;

        cashAccountRow.id = account.getId();
        return {accountRow, cashAccountRow};
    }

}   // namespace app