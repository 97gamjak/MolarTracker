#include "store/position_store.hpp"

#include <memory>

#include "exceptions/not_yet_implemented.hpp"
#include "finance/account/accounts.hpp"
#include "finance/filter/predicates.hpp"
#include "finance/positions.hpp"
#include "logging/log_macros.hpp"
#include "store/base/base_store.hpp"

REGISTER_LOG_CATEGORY("Store.PositionStore");

namespace store
{

    /**
     * @brief Construct a new Position Store:: Position Store object
     *
     * @param positionService
     */
    PositionStore::PositionStore(
        std::shared_ptr<service::IPositionService> positionService
    )
        : _positionService(std::move(positionService))
    {
    }

    PositionStore::~PositionStore() = default;

    /**
     * @brief Create a new position
     *
     * @param position
     *
     * @return PositionId
     */
    PositionId PositionStore::createPosition(const finance::Position& position)
    {
        return _addEntry(position);
    }

    /**
     * @brief Get all positions
     *
     * @param accounts The accounts view to filter positions by
     * @return finance::Positions
     */
    finance::Positions PositionStore::getAllPositions(
        const finance::AccountsView& accounts
    ) const
    {
        const auto accountIds = accounts.getIds();

        if (accountIds.empty())
            return {};

        auto options = Options{.deletion = DeletionPolicy::ExcludeDelete};

        finance::Positions positions;
        for (const auto& position : _getValues(options))
            positions.addUnchecked(position);

        options.deletion = DeletionPolicy::IncludeDelete;
        const auto ids   = _getIds(options);

        const auto dbPositions = _positionService->getAllPositions(accountIds);

        for (const auto& position : dbPositions)
            if (!ids.contains(position.getId()))
                positions.addUnchecked(position);

        return positions;
    }

    /**
     * @brief Get all open positions
     *
     * @param accountIds The set of account IDs to filter positions by
     * @return finance::Positions
     */
    finance::Positions PositionStore::getOpenPositions(
        const IdSet<AccountId>& accountIds
    ) const
    {
        LOG_ENTRY;

        if (accountIds.empty())
            return {};

        auto options = Options{
            .filter = finance::IsPositionOpen() &&
                      finance::IsPositionForAccounts(accountIds),
            .deletion = DeletionPolicy::ExcludeDelete
        };

        finance::Positions positions;

        for (const auto& position : _getValues(options))
            positions.addUnchecked(position);

        options.deletion = DeletionPolicy::IncludeDelete;
        const auto ids   = _getIds(options);

        const auto openPositions =
            _positionService->getAllOpenPositions(accountIds);

        for (const auto& position : openPositions)
            if (!ids.contains(position.getId()))
                positions.addUnchecked(position);

        LOG_DEBUG(
            std::format("Open positions retrieved: {}", positions.size())
        );

        return positions;
    }

    /**
     * @brief Get a position by its ID
     *
     * @param positionId The ID of the position to retrieve
     * @return std::optional<finance::Position>
     */
    std::optional<finance::Position> PositionStore::getPosition(
        PositionId positionId
    ) const
    {
        LOG_ENTRY;

        auto options = Options{
            .filter   = finance::checkId<finance::Position>(positionId),
            .deletion = DeletionPolicy::IncludeDelete
        };

        auto positions = _getValues(options);

        std::vector<finance::Position> positionsVec(
            positions.begin(),
            positions.end()
        );

        if (positionsVec.size() > 1)
        {
            throw std::runtime_error(
                std::format(
                    "Multiple positions found for position ID: {}",
                    positionId.toString()
                )
            );
        }

        if (!positionsVec.empty() && positionsVec.size() == 1)
            return positionsVec.front();

        return _positionService->getPosition(positionId);
    }

    /**
     * @brief Commit all changes to the position store
     *
     */
    void PositionStore::commit()
    {
        _logCache(LOG_CATEGORY, LogLevel::Trace);

        for (const auto& entry : _getEntries())
        {
            switch (entry.state)
            {
                case StoreState::New:
                {
                    const auto id =
                        _positionService->createPosition(entry.value);
                    auto newEntry = entry;
                    newEntry.value.setId(id);

                    _commitEntry(entry.value.getId(), newEntry);
                    break;
                }
                case StoreState::Clean:
                    break;
                case StoreState::Deleted:
                case StoreState::Modified:
                {
                    throw NotYetImplementedException(
                        "Position modification is not yet implemented"
                    );
                }
            }
        }
    }

    /**
     * @brief Subscribe to position closed events, this allows subscribers to be
     * notified when a position is closed, which can be useful for updating
     *
     * @param func
     * @param user
     * @return Connection
     */
    Connection PositionStore::subscribeToPositionClosed(
        PositionClosed::func func,
        void*                user
    )
    {
        return _positionEvents->template on<PositionClosed>(
            std::move(func),
            user
        );
    }

}   // namespace store
