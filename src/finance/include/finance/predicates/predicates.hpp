#ifndef __FINANCE__INCLUDE__FINANCE__PREDICATES__PREDICATES_HPP__
#define __FINANCE__INCLUDE__FINANCE__PREDICATES__PREDICATES_HPP__

#include "common/container/set.hpp"
#include "config/id_types.hpp"
#include "filter/predicate.hpp"

namespace finance
{
    template <typename T>
    concept HasInstrumentIdConcept = requires(T item) {
        { item.getInstrumentId() };
    };

    template <HasInstrumentIdConcept T>
    filter::Predicate<T> HasInstrumentId(InstrumentId id);

    template <HasInstrumentIdConcept T>
    filter::Predicate<T> HasInstrumentIds(const IdSet<InstrumentId>& ids);
}   // namespace finance

#ifndef __FINANCE__INCLUDE__FINANCE__PREDICATES__PREDICATES_TPP__
#include "predicates.tpp"   // IWYU pragma: export
#endif   // __FINANCE__INCLUDE__FINANCE__PREDICATES__PREDICATES_TPP__

#endif   // __FINANCE__INCLUDE__FINANCE__PREDICATES__PREDICATES_HPP__