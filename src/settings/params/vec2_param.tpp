#ifndef __SETTINGS__PARAMS__VEC2_PARAM_TPP__
#define __SETTINGS__PARAMS__VEC2_PARAM_TPP__

#include "vec2_param.hpp"

namespace settings
{
    template <typename T>
    requires(std::integral<T> || std::floating_point<T>)
    Vec2Param<T>::Vec2Param(
        std::string key,
        std::string title,
        std::string description,
        std::string xKey,
        std::string xTitle,
        std::string yKey,
        std::string yTitle
    )
        : _core{std::move(key), std::move(title), std::move(description)}
        , _x{std::move(xKey), std::move(xTitle)}
        , _y{std::move(yKey), std::move(yTitle)}
    {
    }

    template <typename T>
    requires(std::integral<T> || std::floating_point<T>)
    NumericParam<T>& Vec2Param<T>::x()
    {
        return _x;
    }

    template <typename T>
    requires(std::integral<T> || std::floating_point<T>)
    const NumericParam<T>& Vec2Param<T>::x() const
    {
        return _x;
    }

    template <typename T>
    requires(std::integral<T> || std::floating_point<T>)
    NumericParam<T>& Vec2Param<T>::y()
    {
        return _y;
    }

    template <typename T>
    requires(std::integral<T> || std::floating_point<T>)
    const NumericParam<T>& Vec2Param<T>::y() const
    {
        return _y;
    }

    template <typename T>
    requires(std::integral<T> || std::floating_point<T>)
    const T& Vec2Param<T>::getX() const
    {
        return _x.get();
    }

    template <typename T>
    requires(std::integral<T> || std::floating_point<T>)
    const T& Vec2Param<T>::getY() const
    {
        return _y.get();
    }

    template <typename T>
    requires(std::integral<T> || std::floating_point<T>)
    std::expected<void, ParamError> Vec2Param<T>::setX(const T& value)
    {
        return _x.set(value);
    }

    template <typename T>
    requires(std::integral<T> || std::floating_point<T>)
    std::expected<void, ParamError> Vec2Param<T>::setY(const T& value)
    {
        return _y.set(value);
    }

    template <typename T>
    requires(std::integral<T> || std::floating_point<T>)
    template <typename Func>
    void Vec2Param<T>::_forEachParam(Func&& func) const
    {
        std::forward<Func>(func)(_x);
        std::forward<Func>(func)(_y);
    }

    template <typename T>
    requires(std::integral<T> || std::floating_point<T>)
    template <typename Func>
    void Vec2Param<T>::_forEachParam(Func&& func)
    {
        std::forward<Func>(func)(_x);
        std::forward<Func>(func)(_y);
    }

}   // namespace settings

#endif   // __SETTINGS__PARAMS__VEC2_PARAM_TPP__
