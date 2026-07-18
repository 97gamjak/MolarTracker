#ifndef __FINANCE__INCLUDE__FINANCE__TRANSACTION__I_SECURITY_TRANSACTION_HPP__
#define __FINANCE__INCLUDE__FINANCE__TRANSACTION__I_SECURITY_TRANSACTION_HPP__

#include "config/id_types.hpp"

namespace finance
{
    /**
     * @brief Interface for security transactions.
     *
     * This interface defines the common behavior for all security
     * transactions, including stocks and bonds.
     */
    class ISecurityTransaction
    {
       public:
        virtual ~ISecurityTransaction() = default;

    };
}   // namespace finance

#endif   // __FINANCE__INCLUDE__FINANCE__TRANSACTION__I_SECURITY_TRANSACTION_HPP__