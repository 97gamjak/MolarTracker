#include "account_factory.hpp"

#include "config/id_types.hpp"
#include "finance/cash_account.hpp"
#include "sql_models/account_row.hpp"

namespace app
{

    /**
     * @brief Convert a CashAccountRow database model to a
     * CashAccount
     *
     * @param cashAccountRow
     * @return finance::CashAccount
     */
    finance::CashAccount CashAccountFactory::toDomain(
        const CashAccountRow& cashAccountRow
    )
    {
        return finance::CashAccount(
            cashAccountRow.id.value(),
            cashAccountRow.status.value(),
            cashAccountRow.profileId.value(),
            cashAccountRow.currency.value(),
            cashAccountRow.name.value()
        );
    }

    /**
     * @brief Convert a vector of CashAccountRow database models to a vector of
     * CashAccount domain models
     *
     * @param cashAccountRows
     * @return std::vector<finance::CashAccount>
     */
    std::vector<finance::CashAccount> CashAccountFactory::toDomains(
        const std::vector<CashAccountRow>& cashAccountRows
    )
    {
        std::vector<finance::CashAccount> accounts;
        accounts.reserve(cashAccountRows.size());

        for (const auto& row : cashAccountRows)
            accounts.push_back(toDomain(row));

        return accounts;
    }

    /**
     * @brief Convert a CashAccount domain model to a pair of AccountRow and
     * CashAccountRow database models, this method takes a CashAccount object as
     * input and extracts the relevant information to populate an AccountRow and
     * a CashAccountRow, it returns a pair containing both rows.
     *
     * @param account
     * @return std::pair<AccountRow, CashAccountRow>
     */
    std::pair<AccountRow, CashAccountRow> CashAccountFactory::toRow(
        const finance::CashAccount& account
    )
    {
        AccountRow     accountRow;
        CashAccountRow cashAccountRow;

        accountRow.id   = account.getId();
        accountRow.kind = AccountKind::Cash;

        cashAccountRow.id        = account.getId();
        cashAccountRow.status    = account.getStatus();
        cashAccountRow.profileId = account.getProfileId();
        cashAccountRow.currency  = account.getCurrency();
        cashAccountRow.name      = account.getName();

        return {accountRow, cashAccountRow};
    }

}   // namespace app