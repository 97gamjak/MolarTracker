#ifndef __SERVICE__SRC__SERVICE__INSTRUMENT_SERVICE_HPP__
#define __SERVICE__SRC__SERVICE__INSTRUMENT_SERVICE_HPP__

#include <memory>

#include "service/i_instrument_service.hpp"

namespace finance
{
    class Stock;                    // forward declaration
    struct StockInsertionResult;    // forward declaration
    class Option;                   // forward declaration
    struct OptionInsertionResult;   // forward declaration
}   // namespace finance

namespace repo
{
    class IInstrumentRepo;   // forward declaration
}   // namespace repo

namespace service
{

    /**
     * @brief Implementation of Instrument service
     *
     */
    class InstrumentService : public IInstrumentService
    {
       private:
        /// reference to the instrument repository
        std::shared_ptr<repo::IInstrumentRepo> _instrumentRepo;

       public:
        explicit InstrumentService(
            const std::shared_ptr<repo::IInstrumentRepo>& instrumentRepo
        );

        [[nodiscard]]
        std::vector<finance::Option> getOptions() override;

        [[nodiscard]]
        finance::Stocks getStocks(const finance::StockFilter& filter) override;

        [[nodiscard]]
        finance::StockInsertionResult addStock(
            const finance::Stock& stock
        ) override;

        [[nodiscard]]
        finance::OptionInsertionResult addOption(
            const finance::Option& option
        ) override;

        [[nodiscard]] bool stockExists(const std::string& ticker) override;

        [[nodiscard]] bool optionExists(const finance::Option& option) override;
    };

}   // namespace service

#endif   // __SERVICE__SRC__SERVICE__INSTRUMENT_SERVICE_HPP__