#ifndef __SETTINGS__PARAMS__VECN_PARAM_TPP__
#define __SETTINGS__PARAMS__VECN_PARAM_TPP__

#include "vecn_param.hpp"

namespace settings
{
    // ── Private helpers ──────────────────────────────────────────────────────

    template <typename T, std::size_t N>
    requires(N >= 2 && (std::integral<T> || std::floating_point<T>))
    template <std::size_t... Is>
    std::array<NumericParam<T>, N> VecNParam<T, N>::_makeComponents(
        const std::array<std::string, N>& keys,
        const std::array<std::string, N>& titles,
        std::index_sequence<Is...>
    )
    {
        return {NumericParam<T>{keys[Is], titles[Is]}...};
    }

    template <typename T, std::size_t N>
    requires(N >= 2 && (std::integral<T> || std::floating_point<T>))
    std::array<std::string, N> VecNParam<T, N>::_defaultKeys()
    {
        std::array<std::string, N> keys;
        for (std::size_t i = 0; i < N; ++i)
            keys[i] = (i < 4) ? _defaultKeyNames[i] : std::to_string(i);
        return keys;
    }

    template <typename T, std::size_t N>
    requires(N >= 2 && (std::integral<T> || std::floating_point<T>))
    std::array<std::string, N> VecNParam<T, N>::_defaultTitles()
    {
        std::array<std::string, N> titles;
        for (std::size_t i = 0; i < N; ++i)
            titles[i] = (i < 4) ? _defaultTitleNames[i] : std::to_string(i);
        return titles;
    }

    // ── Constructors ─────────────────────────────────────────────────────────

    template <typename T, std::size_t N>
    requires(N >= 2 && (std::integral<T> || std::floating_point<T>))
    VecNParam<T, N>::VecNParam(
        std::string                key,
        std::string                title,
        std::string                description,
        std::array<std::string, N> componentKeys,
        std::array<std::string, N> componentTitles
    )
        : _core{std::move(key), std::move(title), std::move(description)}
        , _components{_makeComponents(
              componentKeys,
              componentTitles,
              std::make_index_sequence<N>{}
          )}
    {
    }

    template <typename T, std::size_t N>
    requires(N >= 2 && (std::integral<T> || std::floating_point<T>))
    VecNParam<T, N>::VecNParam(
        std::string key,
        std::string title,
        std::string description
    )
        : VecNParam{
              std::move(key),
              std::move(title),
              std::move(description),
              _defaultKeys(),
              _defaultTitles()}
    {
    }

    // ── Generic indexed access ────────────────────────────────────────────────

    template <typename T, std::size_t N>
    requires(N >= 2 && (std::integral<T> || std::floating_point<T>))
    NumericParam<T>& VecNParam<T, N>::component(std::size_t i)
    {
        return _components[i];
    }

    template <typename T, std::size_t N>
    requires(N >= 2 && (std::integral<T> || std::floating_point<T>))
    const NumericParam<T>& VecNParam<T, N>::component(std::size_t i) const
    {
        return _components[i];
    }

    template <typename T, std::size_t N>
    requires(N >= 2 && (std::integral<T> || std::floating_point<T>))
    const T& VecNParam<T, N>::get(std::size_t i) const
    {
        return _components[i].get();
    }

    template <typename T, std::size_t N>
    requires(N >= 2 && (std::integral<T> || std::floating_point<T>))
    std::expected<void, ParamError> VecNParam<T, N>::set(
        std::size_t i,
        const T&    value
    )
    {
        return _components[i].set(value);
    }

    // ── Named sub-param accessors ─────────────────────────────────────────────

    template <typename T, std::size_t N>
    requires(N >= 2 && (std::integral<T> || std::floating_point<T>))
    NumericParam<T>& VecNParam<T, N>::x()
    {
        return _components[0];
    }

    template <typename T, std::size_t N>
    requires(N >= 2 && (std::integral<T> || std::floating_point<T>))
    const NumericParam<T>& VecNParam<T, N>::x() const
    {
        return _components[0];
    }

    template <typename T, std::size_t N>
    requires(N >= 2 && (std::integral<T> || std::floating_point<T>))
    NumericParam<T>& VecNParam<T, N>::y()
    {
        return _components[1];
    }

    template <typename T, std::size_t N>
    requires(N >= 2 && (std::integral<T> || std::floating_point<T>))
    const NumericParam<T>& VecNParam<T, N>::y() const
    {
        return _components[1];
    }

    template <typename T, std::size_t N>
    requires(N >= 2 && (std::integral<T> || std::floating_point<T>))
    NumericParam<T>& VecNParam<T, N>::z() requires(N >= 3)
    {
        return _components[2];
    }

    template <typename T, std::size_t N>
    requires(N >= 2 && (std::integral<T> || std::floating_point<T>))
    const NumericParam<T>& VecNParam<T, N>::z() const requires(N >= 3)
    {
        return _components[2];
    }

    template <typename T, std::size_t N>
    requires(N >= 2 && (std::integral<T> || std::floating_point<T>))
    NumericParam<T>& VecNParam<T, N>::w() requires(N >= 4)
    {
        return _components[3];
    }

    template <typename T, std::size_t N>
    requires(N >= 2 && (std::integral<T> || std::floating_point<T>))
    const NumericParam<T>& VecNParam<T, N>::w() const requires(N >= 4)
    {
        return _components[3];
    }

    // ── Named value getters ───────────────────────────────────────────────────

    template <typename T, std::size_t N>
    requires(N >= 2 && (std::integral<T> || std::floating_point<T>))
    const T& VecNParam<T, N>::getX() const
    {
        return _components[0].get();
    }

    template <typename T, std::size_t N>
    requires(N >= 2 && (std::integral<T> || std::floating_point<T>))
    const T& VecNParam<T, N>::getY() const
    {
        return _components[1].get();
    }

    template <typename T, std::size_t N>
    requires(N >= 2 && (std::integral<T> || std::floating_point<T>))
    const T& VecNParam<T, N>::getZ() const requires(N >= 3)
    {
        return _components[2].get();
    }

    template <typename T, std::size_t N>
    requires(N >= 2 && (std::integral<T> || std::floating_point<T>))
    const T& VecNParam<T, N>::getW() const requires(N >= 4)
    {
        return _components[3].get();
    }

    // ── Named value setters ───────────────────────────────────────────────────

    template <typename T, std::size_t N>
    requires(N >= 2 && (std::integral<T> || std::floating_point<T>))
    std::expected<void, ParamError> VecNParam<T, N>::setX(const T& v)
    {
        return _components[0].set(v);
    }

    template <typename T, std::size_t N>
    requires(N >= 2 && (std::integral<T> || std::floating_point<T>))
    std::expected<void, ParamError> VecNParam<T, N>::setY(const T& v)
    {
        return _components[1].set(v);
    }

    template <typename T, std::size_t N>
    requires(N >= 2 && (std::integral<T> || std::floating_point<T>))
    std::expected<void, ParamError> VecNParam<T, N>::setZ(const T& v)
        requires(N >= 3)
    {
        return _components[2].set(v);
    }

    template <typename T, std::size_t N>
    requires(N >= 2 && (std::integral<T> || std::floating_point<T>))
    std::expected<void, ParamError> VecNParam<T, N>::setW(const T& v)
        requires(N >= 4)
    {
        return _components[3].set(v);
    }

    // ── ParamGroupMixin requirements ──────────────────────────────────────────

    template <typename T, std::size_t N>
    requires(N >= 2 && (std::integral<T> || std::floating_point<T>))
    template <typename Func>
    void VecNParam<T, N>::_forEachParam(Func&& func) const
    {
        for (const auto& c : _components)
            func(c);
    }

    template <typename T, std::size_t N>
    requires(N >= 2 && (std::integral<T> || std::floating_point<T>))
    template <typename Func>
    void VecNParam<T, N>::_forEachParam(Func&& func)
    {
        for (auto& c : _components)
            func(c);
    }

}   // namespace settings

#endif   // __SETTINGS__PARAMS__VECN_PARAM_TPP__
