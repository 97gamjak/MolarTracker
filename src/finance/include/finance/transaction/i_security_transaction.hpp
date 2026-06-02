#ifndef __FINANCE__INCLUDE__FINANCE__TRANSACTION__I_SECURITY_TRANSACTION_HPP__
#define __FINANCE__INCLUDE__FINANCE__TRANSACTION__I_SECURITY_TRANSACTION_HPP__

#include "config/id_types.hpp"
#include "config/quantity.hpp"

namespace finance
{
    class ISecurityTransaction
    {
       public:
        virtual ~ISecurityTransaction() = default;

        [[nodiscard]]
        virtual InstrumentId getBaseInstrumentId() const = 0;

        [[nodiscard]]
        virtual const Quantity& getQuantity() const = 0;
    };
}   // namespace finance

#endif   // __FINANCE__INCLUDE__FINANCE__TRANSACTION__I_SECURITY_TRANSACTION_HPP__