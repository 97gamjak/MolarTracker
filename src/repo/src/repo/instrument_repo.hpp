#ifndef __REPO__SRC__REPO__INSTRUMENT_REPO_HPP__
#define __REPO__SRC__REPO__INSTRUMENT_REPO_HPP__

#include "base_repo.hpp"
#include "config/id_types.hpp"
#include "finance/instrument/option.hpp"
#include "repo/i_instrument_repo.hpp"
#include "sql_models/instrument_row.hpp"
#include "sql_models/stock_row.hpp"

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
        std::vector<std::string> getTickers() override;

        [[nodiscard]]
        std::vector<finance::Stock> getStocks(
            const idSet<InstrumentId>& ids
        ) override;

        [[nodiscard]]
        std::vector<finance::Option> getOptions() override;

        [[nodiscard]]
        std::optional<finance::Stock> getStock(
            const std::string& ticker
        ) override;

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
        std::vector<StockRow> _getStockRows(
            const idSet<InstrumentId>& ids = {}
        );

        [[nodiscard]]
        InstrumentId _addInstrument(const InstrumentRow& instrumentRow);
    };
}   // namespace repo

#endif   // __REPO__SRC__REPO__INSTRUMENT_REPO_HPP__