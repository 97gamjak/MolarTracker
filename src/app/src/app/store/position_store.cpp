#include "app/store/position_store.hpp"

#include "app/store/base/base_store.hpp"
#include "exceptions/not_yet_implemented.hpp"

namespace app
{

    /**
     * @brief Construct a new Position Store:: Position Store object
     *
     * @param positionService
     */
    PositionStore::PositionStore(
        std::shared_ptr<IPositionService> positionService
    )
        : _positionService(std::move(positionService))
    {
        const auto openPositions = _positionService->getAllOpenPositions();

        _addCleanEntries(openPositions);
    }

    /**
     * @brief Create a new position
     *
     * @param position
     * @return PositionId
     */
    PositionId PositionStore::createPosition(const finance::Position& position)
    {
        _addEntry(position);

        return position.getId();
    }

    /**
     * @brief Get all positions
     *
     * @return std::vector<finance::Position>
     */
    std::vector<finance::Position> PositionStore::getAllPositions() const
    {
        auto options = Options{.deletion = DeletionPolicy::ExcludeDelete};

        auto                           positionsView = _getValues(options);
        std::vector<finance::Position> positions     = {
            positionsView.begin(),
            positionsView.end()
        };

        options.deletion = DeletionPolicy::IncludeDelete;
        const auto ids   = _getIds(options);

        for (const auto& position : _positionService->getAllPositions())
            if (!ids.contains(position.getId()))
                positions.push_back(position);

        return positions;
    }

    /**
     * @brief Commit all changes to the position store
     *
     */
    void PositionStore::commit()
    {
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

        _notifyOnCommit();
    }

}   // namespace app
