#include "finance/position.hpp"

#include <format>

#include "filter/predicate.hpp"
#include "utils/container/set.hpp"

namespace finance
{
    /**
     * @brief Construct a new Position:: Position object
     *
     * @param accountId The ID of the account associated with the position.
     * @param createdAt The creation timestamp of the position.
     * @param closedAt The closing timestamp of the position, if it exists.
     */
    Position::Position(
        AccountId                accountId,
        Timestamp                createdAt,
        std::optional<Timestamp> closedAt
    )
        : _accountId(accountId), _createdAt(createdAt), _closedAt(closedAt)
    {
    }

    /**
     * @brief Set the ID of the position.
     *
     * @param id The new ID.
     */
    void Position::setId(PositionId id) { _id = id; }

    /**
     * @brief Get the ID of the position.
     *
     * @return PositionId
     */
    PositionId Position::getId() const { return _id; }

    /**
     * @brief Get the creation timestamp of the position.
     *
     * @return Timestamp
     */
    Timestamp Position::getCreatedAt() const { return _createdAt; }

    /**
     * @brief Get the closing timestamp of the position, if it exists.
     *
     * @return std::optional<Timestamp>
     */
    std::optional<Timestamp> Position::getClosedAt() const { return _closedAt; }

    /**
     * @brief Get the account ID associated with the position.
     *
     * @return AccountId
     */
    AccountId Position::getAccountId() const { return _accountId; }

    /**
     * @brief Predicate to check if a position is open (i.e. has no closing
     * timestamp)
     *
     * @return filter::Predicate<Position>
     */
    filter::Predicate<Position> IsPositionOpen(bool isOpen)
    {
        return filter::Predicate<Position>{
            [isOpen](const Position& position)
            { return isOpen == !position.getClosedAt().has_value(); }
        };
    }

    /**
     * @brief Predicate to check if a position is associated with any of the
     * given account IDs.
     *
     * @return filter::Predicate<Position>
     */
    filter::Predicate<Position> IsPositionForAccounts(
        const IdSet<AccountId>& accountIds
    )
    {
        return filter::Predicate<Position>{
            [accountIds](const Position& position)
            { return accountIds.contains(position.getAccountId()); }
        };
    }

    /**
     * @brief Get a string representation of the position.
     *
     * @return std::string
     */
    std::string Position::toString() const
    {
        return std::format(
            "Position(id={}, createdAt={}, closedAt={})",
            _id.toString(),
            _createdAt.humanReadable(),
            _closedAt.has_value() ? _closedAt->humanReadable() : "null"
        );
    }

}   // namespace finance