#ifndef __FINANCE__INCLUDE__FINANCE__FILTER__PREDICATES_TPP__
#define __FINANCE__INCLUDE__FINANCE__FILTER__PREDICATES_TPP__

#include "predicates.hpp"

namespace finance
{
    template <HasInstrumentId T>
    filter::Predicate<T> checkInstrumentId(const IdSet<InstrumentId>& ids)
    {
        return filter::makePredicate<T>(
            [ids](const T& item)
            { return ids.contains(item.getInstrumentId()); }
        );
    }

    template <HasId T>
    filter::Predicate<T> checkId(const IdOf<T>& id)
    {
        return filter::makePredicate<T>([id](const T& item)
                                        { return item.getId() == id; });
    }

    template <HasId T>
    filter::Predicate<T> checkId(const IdSet<IdOf<T>>& ids)
    {
        return filter::makePredicate<T>([ids](const T& item)
                                        { return ids.contains(item.getId()); });
    }

}   // namespace finance

#endif   // __FINANCE__INCLUDE__FINANCE__FILTER__PREDICATES_TPP__