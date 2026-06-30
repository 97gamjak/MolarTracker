#ifndef __FINANCE__INCLUDE__FINANCE__FILTER__PREDICATES_HPP__
#define __FINANCE__INCLUDE__FINANCE__FILTER__PREDICATES_HPP__

#include "config/id_types.hpp"
#include "filter/predicate.hpp"
#include "utils/container/set.hpp"

namespace finance
{
    template <HasInstrumentId T>
    [[nodiscard]]
    filter::Predicate<T> checkInstrumentId(const IdSet<InstrumentId>& ids);

    template <HasId T>
    [[nodiscard]]
    filter::Predicate<T> checkId(const IdOf<T>& id);

    template <HasId T>
    [[nodiscard]]
    filter::Predicate<T> checkId(const IdSet<IdOf<T>>& ids);

}   // namespace finance

#ifndef __FINANCE__INCLUDE__FINANCE__FILTER__PREDICATES_TPP__
#include "predicates.tpp"   // IWYU pragma: export
#endif   // __FINANCE__INCLUDE__FINANCE__FILTER__PREDICATES_TPP__

#endif   // __FINANCE__INCLUDE__FINANCE__FILTER__PREDICATES_HPP__