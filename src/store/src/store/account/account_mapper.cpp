#include "account_mapper.hpp"

#include "domain/account.hpp"
#include "logic/finance/account.hpp"

namespace store
{

    /**
     * @brief Maps a finance account to a domain account.
     *
     * @param account The finance account to map.
     * @return The mapped domain account.
     */
    domain::Account AccountMapper::toDomain(const finance::Account& account)
    {
        return domain::Account{
            account.getId(),
            account.getStatus(),
            account.getName(),
            account.getCurrency(),
            account.getKind()
        };
    }

    /**
     * @brief Maps a domain account to a finance account.
     *
     * @param domain The domain account to map.
     * @return The mapped finance account.
     */
    finance::Account AccountMapper::fromDomain(const domain::Account& domain)
    {
        return finance::Account{
            domain.getId(),
            domain.getStatus(),
            domain.getName(),
            domain.getCurrency(),
            domain.getKind()
        };
    }

}   // namespace store
