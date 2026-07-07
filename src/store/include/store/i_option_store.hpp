#ifndef __STORE__INCLUDE__STORE__I_OPTION_STORE_HPP__
#define __STORE__INCLUDE__STORE__I_OPTION_STORE_HPP__

#include "config/id_types.hpp"
#include "error/finance_error.hpp"

namespace finance
{
    class Option;   // forward declaration
}   // namespace finance

namespace store
{
    /**
     * @brief Read Only interface for the OptionStore, this interface defines
     * the read-only operations that can be performed on the option store,
     * allowing for a structured way to access options within the application
     * without modifying the underlying data storage for options.
     *
     */
    class IOptionStoreReader
    {
       public:
        virtual ~IOptionStoreReader() = default;
    };

    /**
     * @brief Store for managing options, this interface defines the operations
     * that can be performed on the option store, such as adding new options,
     * retrieving the mapping of instrument IDs, and committing changes to the
     * store, allowing for a structured way to manage options within the
     * application and interact with the underlying data storage for options.
     *
     */
    class IOptionStore
    {
       public:
        virtual ~IOptionStore() = default;

        /**
         * @brief Add a new option to the store, this will insert the option
         * into the store and return the generated instrument ID for the option
         * if the operation is successful, or an error result if the option
         * already exists or if there was an error during the operation.
         *
         * @param option The Option object containing the details of the option
         * to add to the store
         * @return FinanceResult<InstrumentId> The generated
         * InstrumentId if successful, or an OptionStoreResult error code if
         * there was an error or if the option already exists in the store
         */
        [[nodiscard]]
        virtual FinanceResult<InstrumentId> addOption(
            finance::Option option
        ) = 0;
    };
}   // namespace store

#endif   // __STORE__INCLUDE__STORE__I_OPTION_STORE_HPP__