#include "store/position_store.hpp"

#include <memory>

#include "exceptions/not_yet_implemented.hpp"
#include "store/account/account_session.hpp"
#include "store/base/base_store.hpp"

namespace store
{

    /**
     * @brief Session data for PositionStore
     *
     */
    struct PositionStore::Session
    {
        /// the account session
        const AccountSession& accountSession;

        /**
         * @brief Construct a new Session object
         *
         * @param accountSession_
         */
        explicit Session(const AccountSession& accountSession_)
            : accountSession(accountSession_)
        {
        }

        ~Session() = default;

        // delete copy and move
        Session(const Session&)            = delete;
        Session(Session&&)                 = delete;
        Session& operator=(const Session&) = delete;
        Session& operator=(Session&&)      = delete;
    };

    /**
     * @brief Construct a new Position Store:: Position Store object
     *
     * @param positionService
     * @param accountSession
     */
    PositionStore::PositionStore(
        std::shared_ptr<service::IPositionService> positionService,
        const AccountSession&                      accountSession
    )
        : _positionService(std::move(positionService)),
          _session(std::make_unique<Session>(accountSession))
    {
        const auto accountIds = _session->accountSession.getIds();
        if (!accountIds.empty())
        {
            const auto openPositions =
                _positionService->getAllOpenPositions(accountIds);

            _addCleanEntries(openPositions);
        }
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
     * @return std::vector<finance::Position>
     */
    std::vector<finance::Position> PositionStore::getAllPositions() const
    {
        const auto accountIds = _session->accountSession.getIds();

        if (accountIds.empty())
            return {};

        auto options = Options{.deletion = DeletionPolicy::ExcludeDelete};

        auto                           positionsView = _getValues(options);
        std::vector<finance::Position> positions     = {
            positionsView.begin(),
            positionsView.end()
        };

        options.deletion = DeletionPolicy::IncludeDelete;
        const auto ids   = _getIds(options);

        const auto dbPositions = _positionService->getAllPositions(accountIds);

        for (const auto& position : dbPositions)
            if (!ids.contains(position.getId()))
                positions.push_back(position);

        return positions;
    }

    /**
     * @brief Get all open positions
     *
     * @return std::vector<finance::Position>
     */
    std::vector<finance::Position> PositionStore::getOpenPositions() const
    {
        const auto accountIds = _session->accountSession.getIds();

        if (accountIds.empty())
            return {};

        auto options = Options{
            .filter   = finance::IsPositionOpen(),
            .deletion = DeletionPolicy::ExcludeDelete
        };

        auto                           positionsView = _getValues(options);
        std::vector<finance::Position> positions     = {
            positionsView.begin(),
            positionsView.end()
        };

        options.deletion = DeletionPolicy::IncludeDelete;
        const auto ids   = _getIds(options);

        const auto openPositions =
            _positionService->getAllOpenPositions(accountIds);

        for (const auto& position : openPositions)
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

}   // namespace store
