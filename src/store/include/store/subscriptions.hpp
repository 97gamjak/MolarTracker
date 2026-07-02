#ifndef __STORE__INCLUDE__STORE__SUBSCRIPTIONS_HPP__
#define __STORE__INCLUDE__STORE__SUBSCRIPTIONS_HPP__

#include "config/signal_tags.hpp"       // IWYU pragma: keep
#include "connections/connection.hpp"   // IWYU pragma: keep

#define SUBSCRIBE_VIRTUAL(x, id)                         \
                                                         \
    [[nodiscard]]                                        \
    virtual Connection subscribeToAdded(                 \
        typename OnStoreItemAdded<x>::func func,         \
        void*                              subscriber    \
    ) = 0;                                               \
                                                         \
    [[nodiscard]]                                        \
    virtual Connection subscribeToUpdated(               \
        typename OnStoreItemUpdated<x>::func func,       \
        void*                                subscriber  \
    ) = 0;                                               \
                                                         \
    [[nodiscard]]                                        \
    virtual Connection subscribeToRemoved(               \
        typename OnStoreItemRemoved<id>::func func,      \
        void*                                 subscriber \
    ) = 0;

#define SUBSCRIBE_OVERRIDE(x, id)                                           \
                                                                            \
    [[nodiscard]]                                                           \
    Connection subscribeToAdded(                                            \
        typename OnStoreItemAdded<x>::func func,                            \
        void*                              subscriber                       \
    ) override                                                              \
    {                                                                       \
        return BaseStore<x, id>::subscribeToEntryAdded(func, subscriber);   \
    }                                                                       \
                                                                            \
    [[nodiscard]]                                                           \
    Connection subscribeToUpdated(                                          \
        typename OnStoreItemUpdated<x>::func func,                          \
        void*                                subscriber                     \
    ) override                                                              \
    {                                                                       \
        return BaseStore<x, id>::subscribeToEntryUpdated(func, subscriber); \
    }                                                                       \
                                                                            \
    [[nodiscard]]                                                           \
    Connection subscribeToRemoved(                                          \
        typename OnStoreItemRemoved<id>::func func,                         \
        void*                                 subscriber                    \
    ) override                                                              \
    {                                                                       \
        return BaseStore<x, id>::subscribeToEntryRemoved(func, subscriber); \
    }

#endif   // __STORE__INCLUDE__STORE__SUBSCRIPTIONS_HPP__