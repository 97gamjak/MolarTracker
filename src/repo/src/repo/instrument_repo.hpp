#ifndef __REPO__SRC__REPO__INSTRUMENT_REPO_HPP__
#define __REPO__SRC__REPO__INSTRUMENT_REPO_HPP__

#include "base_repo.hpp"
#include "config/id_types.hpp"
#include "finance/instrument/option.hpp"
#include "repo/i_instrument_repo.hpp"
#include "sql_models/instrument_row.hpp"

namespace repo
{
    /**
     * @brief Repository for managing instruments in the database
     */
    class InstrumentRepo : public BaseRepo, public IInstrumentRepo
    {
       public:
        using BaseRepo::BaseRepo;

        [[nodiscard]]
        finance::Stocks getStocks(const finance::StockFilter& filter) override;

        [[nodiscard]]
        std::vector<finance::Option> getOptions() override;

        [[nodiscard]]
        finance::StockInsertionResult addStock(
            const finance::Stock& stock
        ) override;

        [[nodiscard]]
        finance::OptionInsertionResult addOption(
            const finance::Option& option
        ) override;

        [[nodiscard]]
        bool stockExists(const std::string& ticker) override;

        [[nodiscard]]
        bool optionExists(const finance::Option& option) override;

       private:
        [[nodiscard]]
        InstrumentId _addInstrument(const InstrumentRow& instrumentRow);
    };
}   // namespace repo

#endif   // __REPO__SRC__REPO__INSTRUMENT_REPO_HPP__