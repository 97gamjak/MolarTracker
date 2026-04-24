#ifndef __APP__INCLUDE__APP__REPOS_API__I_INSTRUMENT_REPO_HPP__
#define __APP__INCLUDE__APP__REPOS_API__I_INSTRUMENT_REPO_HPP__

#include "config/id_types.hpp"
#include "sql_models/instrument_row.hpp"

namespace app
{
    class TransactionRepo;

    /**
     * @brief Interface for the instrument repository
     */
    class IInstrumentRepo
    {
       public:
        virtual ~IInstrumentRepo() = default;

       protected:
        friend class TransactionRepo;

        // we do not want to expose this method to the service layer
        virtual std::optional<InstrumentId> getInstrumentId(
            const InstrumentRow& row
        ) = 0;
    };
}   // namespace app

#endif   // __APP__INCLUDE__APP__REPOS_API__I_INSTRUMENT_REPO_HPP__
