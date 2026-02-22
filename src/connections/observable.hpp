#ifndef __CONNECTIONS__OBSERVABLE_HPP__
#define __CONNECTIONS__OBSERVABLE_HPP__

#include "signal.hpp"

/**
 * @brief Base class for observable objects that can emit signals for multiple
 * event tags, this class uses a tuple of signals to manage the different
 * events that can be emitted by the observable object. It provides a method
 * for subscribing to events and a protected method for emitting events, the
 * exact behavior of these methods will depend on the implementation of the
 * derived class.
 *
 * @tparam Tags A variadic template parameter pack representing the different
 * event tags that the observable object can emit, each tag should have an
 * associated argument type defined as `TagType` that will be passed to the
 * subscribers when the event is emitted.
 */
template <typename... Tags>
class Observable
{
   private:
    /// Tuple of signals for each event tag
    std::tuple<Signal<Tags>...> _signals;

   public:
    template <typename Tag>
    Connection on(typename Signal<Tag>::CallbackFn func, void* user);

   protected:
    template <typename Tag>
    void _emit(const typename Tag::TagType& arg);

   private:
    template <typename Tag>
    Signal<Tag>& _getSignal();

    template <typename Tag>
    const Signal<Tag>& _getSignal() const;
};

#ifndef __CONNECTIONS__OBSERVABLE_TPP__
#include "observable.tpp"
#endif   // __CONNECTIONS__OBSERVABLE_TPP__

#endif   // __CONNECTIONS__OBSERVABLE_HPP__