#ifndef __CONNECTIONS__OBSERVABLE_TPP__
#define __CONNECTIONS__OBSERVABLE_TPP__

#include "observable.hpp"

/**
 * @brief Get the signal for the given event tag, this is a helper method that
 * allows the derived class to access the signal for a specific event tag when
 * subscribing to events or emitting events, it uses a static_assert to ensure
 * that the provided event tag is valid and is one of the tags specified in the
 * template parameter pack of the Observable class
 *
 * @tparam Tag The event tag for which to get the signal, this should be one of
 * the tags specified in the template parameter pack of the Observable class
 * @return Signal<Tag>& A reference to the signal for the given event tag
 */
template <typename... Tags>
template <typename Tag>
Signal<Tag>& Observable<Tags...>::_getSignal()
{
    static_assert((std::is_same_v<Tag, Tags> || ...), "Unknown event tag");
    return std::get<Signal<Tag>>(_signals);
}

/**
 * @brief Get the signal for the given event tag, this is a helper method that
 * allows the derived class to access the signal for a specific event tag when
 * subscribing to events or emitting events, it uses a static_assert to ensure
 * that the provided event tag is valid and is one of the tags specified in the
 * template parameter pack of the Observable class
 *
 * @tparam Tag The event tag for which to get the signal, this should be one of
 * the tags specified in the template parameter pack of the Observable class
 * @return const Signal<Tag>& A const reference to the signal for the given
 * event tag
 */
template <typename... Tags>
template <typename Tag>
const Signal<Tag>& Observable<Tags...>::_getSignal() const
{
    static_assert((std::is_same_v<Tag, Tags> || ...), "Unknown event tag");
    return std::get<Signal<Tag>>(_signals);
}

/**
 * @brief Emit an event for the given event tag, this will call all connected
 * callback functions for the signal associated with the given event tag, it
 * uses a static_assert to ensure that the provided event tag is valid and is
 * one of the tags specified in the template parameter pack of the Observable
 * class
 *
 * @tparam Tag The event tag for which to emit the event, this should be one of
 * the tags specified in the template parameter pack of the Observable class
 * @param arg The argument to pass to the connected callback functions, this
 * should be of the type defined as `arg_type` in the event tag struct
 */
template <typename... Tags>
template <typename Tag>
void Observable<Tags...>::_emit(const typename Tag::TagType& arg)
{
    _getSignal<Tag>().notify(arg);
}

/**
 * @brief Subscribe to an event for the given event tag, this will connect the
 * provided callback function to the signal associated with the given event tag,
 * it uses a static_assert to ensure that the provided event tag is valid and is
 * one of the tags specified in the template parameter pack of the Observable
 * class
 *
 * @tparam Tag The event tag for which to subscribe to the event, this should be
 * one of the tags specified in the template parameter pack of the Observable
 * class
 * @param func The callback function to call when the event is emitted, it
 * should have the signature void(void* user, const typename Tag::arg_type& arg)
 * @param user A user-defined pointer that will be passed to the callback
 * function when it is called, this can be used to provide additional context
 * for the callback function
 * @return Connection An object representing the subscription, this can be used
 * to unsubscribe from events by calling disconnect() on it or by letting it go
 * out of scope
 */
template <typename... Tags>
template <typename Tag>
Connection Observable<Tags...>::on(
    typename Signal<Tag>::CallbackFn func,
    void*                            user
)
{
    return _getSignal<Tag>().connect(func, user);
}

#endif   // __CONNECTIONS__OBSERVABLE_TPP__