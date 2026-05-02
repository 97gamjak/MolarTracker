#ifndef __APP__SRC__APP__REPOS__INSTRUMENT_REPO_HPP__
#define __APP__SRC__APP__REPOS__INSTRUMENT_REPO_HPP__

#include "app/repos/base_repo.hpp"
#include "app/repos_api/i_instrument_repo.hpp"
#include "sql_models/stock_row.hpp"

namespace app
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
        std::vector<finance::Stock> getStocks() override;

        [[nodiscard]]
        std::pair<StockId, InstrumentId> addStock(
            const finance::Stock& stock
        ) override;

        [[nodiscard]]
        bool stockExists(const std::string& ticker) override;

       private:
        [[nodiscard]]
        std::vector<StockRow> _getStockRows();
    };
}   // namespace app

#endif   // __APP__SRC__APP__REPOS__INSTRUMENT_REPO_HPP__