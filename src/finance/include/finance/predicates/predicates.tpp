#ifndef __FINANCE__INCLUDE__FINANCE__PREDICATES__PREDICATES_TPP__
#define __FINANCE__INCLUDE__FINANCE__PREDICATES__PREDICATES_TPP__

#include "predicates.hpp"

namespace finance
{
    template <HasInstrumentIdConcept T>
    filter::Predicate<T> HasInstrumentId(InstrumentId id)
    {
        return filter::Predicate<T>([id](const T& item)
                                    { return item.getInstrumentId() == id; });
    }

    template <HasInstrumentIdConcept T>
    filter::Predicate<T> HasInstrumentIds(const IdSet<InstrumentId>& ids)
    {
        return filter::Predicate<T>(
            [ids](const T& item)
            { return ids.contains(item.getInstrumentId()); }
        );
    }

}   // namespace finance

#endif   // __FINANCE__INCLUDE__FINANCE__PREDICATES__PREDICATES_TPP__