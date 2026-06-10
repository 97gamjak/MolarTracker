#ifndef __FINANCE__INCLUDE__FINANCE__TRANSACTION__DOMAIN_TRANSACTION_TPP__
#define __FINANCE__INCLUDE__FINANCE__TRANSACTION__DOMAIN_TRANSACTION_TPP__

#include <algorithm>

#include "domain_transaction.hpp"

namespace finance
{
    /**
     * @brief Visitor for extracting IDs from cash data
     *
     * @tparam IdType
     * @return A vector of extracted IDs
     */
    template <typename IdType>
    std::vector<IdType> GetIdVisitorBase<
        IdType>::operator()(const CashData& /*cashData*/) const
    {
        return {};
    }

    /**
     * @brief Extracts IDs from trade legs
     *
     * @tparam IdType
     * @tparam Proj
     * @param tradeData The trade data to extract IDs from
     * @param proj The projection function to apply
     *
     * @return A vector of extracted IDs
     */
    template <typename IdType>
    template <typename Proj>
    std::vector<IdType> GetIdVisitorBase<IdType>::fromLegs(
        const TradeData& tradeData,
        Proj             proj
    )
    {
        std::vector<IdType> ids;
        const auto&         legs = tradeData.getLegs();
        ids.reserve(legs.size());
        std::ranges::transform(legs, std::back_inserter(ids), proj);
        return ids;
    }

    /**
     * @brief Constructs a GetIdVisitor
     *
     * @tparam IdType
     * @tparam Proj
     * @param _proj The projection function to apply
     */
    template <typename IdType, typename Proj>
    GetIdVisitor<IdType, Proj>::GetIdVisitor(Proj _proj) : proj(_proj)
    {
    }

    /**
     * @brief Visitor for extracting IDs from trade data
     *
     * @tparam IdType
     * @tparam Proj
     * @param tradeData The trade data to extract IDs from
     *
     * @return A vector of extracted IDs
     */
    template <typename IdType, typename Proj>
    std::vector<IdType> GetIdVisitor<IdType, Proj>::operator()(
        const TradeData& tradeData
    ) const
    {
        return this->fromLegs(tradeData, proj);
    }

    /**
     * @brief Checks if a transaction data contains a specific ID
     *
     * @tparam IdType
     * @tparam Proj
     * @param data The transaction data to check
     * @param map The map of IDs to check against
     * @param proj The projection function to apply
     * @return true if the ID is found, false otherwise
     */
    template <typename IdType, typename Proj>
    bool hasId(
        const TransactionData&                data,
        const unorderedIdMap<IdType, IdType>& map,
        Proj                                  proj
    )
    {
        return std::ranges::any_of(
            std::visit(GetIdVisitor<IdType, Proj>{proj}, data),
            [&map](const IdType& id) { return map.contains(id); }
        );
    }

    /**
     * @brief Checks if a transaction data contains a specific ID
     *
     * @tparam IdType
     * @tparam Proj
     * @param data The transaction data to check
     * @param id The ID to check for
     * @param proj The projection function to apply
     * @return true if the ID is found, false otherwise
     */
    template <typename IdType, typename Proj>
    bool hasId(const TransactionData& data, IdType id, Proj proj)
    {
        return std::ranges::any_of(
            std::visit(GetIdVisitor<IdType, Proj>{proj}, data),
            [&id](const IdType& id_) { return id_ == id; }
        );
    }

}   // namespace finance

#endif   // __FINANCE__INCLUDE__FINANCE__TRANSACTION__DOMAIN_TRANSACTION_TPP__