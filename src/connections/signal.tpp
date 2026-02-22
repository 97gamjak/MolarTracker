#ifndef __CONNECTIONS__SIGNAL_TPP__
#define __CONNECTIONS__SIGNAL_TPP__

#include "connection.hpp"
#include "signal.hpp"
#include "signal_exception.hpp"

/**
 * @brief connect a callback function to the signal, this allows the callback
 * function to be called whenever the signal is emitted.
 *
 * @tparam Tag
 * @param func The callback function to call when the signal is emitted, it
 * should have the signature void(void* user, const TagType& arg)
 * @param user A user-defined pointer that will be passed to the callback
 * function when it is called, this can be used to provide additional context
 * for the callback function
 * @return Connection An object representing the subscription, this can be used
 * to unsubscribe from the signal by calling disconnect() on it or by letting it
 * go out of scope
 */
template <typename Tag>
Connection Signal<Tag>::connect(CallbackFn func, void* user)
{
    if (func == nullptr)
    {
        throw SignalException(
            "Cannot connect to signal with null callback function"
        );
    }

    const auto id = _idCounter++;

    if (_idCounter == 0)
        throw SignalException("Subscriber ID counter overflow for signal");

    _subscribers.emplace(id, Subscriber{func, user});

    return Connection::make(this, id, &Signal::_disconnect);
}

/**
 * @brief emit the signal, this will call all connected callback functions with
 * the provided argument
 *
 * @tparam Tag
 * @param arg
 */
template <typename Tag>
void Signal<Tag>::notify(const TagType& arg)
{
    const auto copy = _subscribers;
    for (auto const& [_, sub] : copy)
        sub.func(sub.user, arg);
}

/**
 * @brief Disconnect a subscriber from the signal, this is called by the
 * Connection object when it is destroyed or when disconnect() is called on it,
 * it removes the subscriber with the given id from the list of subscribers for
 * the signal
 *
 * @tparam Tag
 * @param owner
 * @param id
 */
template <typename Tag>
void Signal<Tag>::_disconnect(void* owner, std::size_t id)
{
    auto* self = static_cast<Signal*>(owner);
    self->_subscribers.erase(id);
}

#endif   // __CONNECTIONS__SIGNAL_TPP__