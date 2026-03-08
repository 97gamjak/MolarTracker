#ifndef __SETTINGS__PARAMS__VECN_PARAM_HPP__
#define __SETTINGS__PARAMS__VECN_PARAM_HPP__

#include <array>
#include <cstddef>
#include <expected>
#include <string>

#include "numeric_param.hpp"
#include "param_container.hpp"
#include "param_error.hpp"
#include "param_group_mixin.hpp"

namespace settings
{
    /**
     * @brief N-dimensional vector parameter group
     *
     * Groups N NumericParam<T> instances into a single logical parameter.
     * Named accessors x()/y() are always available; z() requires N >= 3 and
     * w() requires N >= 4. For higher dimensions use component(i) or get(i)/
     * set(i, v).
     *
     * The group serializes as a JSON object keyed by the component keys, e.g.
     * {"x": 1.0, "y": 2.0} for a Vec2Param<float>.
     *
     * Default component keys are "x","y","z","w" for the first four components
     * and the stringified index for any further ones.
     *
     * Convenience type aliases are provided:
     *   Vec2Param<T>  ≡  VecNParam<T, 2>
     *   Vec3Param<T>  ≡  VecNParam<T, 3>
     *   Vec4Param<T>  ≡  VecNParam<T, 4>
     *
     * @tparam T Numeric type (int, float, double, …)
     * @tparam N Number of dimensions (>= 2)
     */
    template <typename T, std::size_t N>
    requires(N >= 2 && (std::integral<T> || std::floating_point<T>))
    class VecNParam : public ParamGroupMixin<VecNParam<T, N>>
    {
       private:
        friend ParamGroupMixin<VecNParam<T, N>>;

        static constexpr std::array<const char*, 4> _defaultKeyNames = {
            "x", "y", "z", "w"};
        static constexpr std::array<const char*, 4> _defaultTitleNames = {
            "X", "Y", "Z", "W"};

        /// Group metadata
        ParamContainer _core;

        /// Component sub-parameters
        std::array<NumericParam<T>, N> _components;

        template <std::size_t... Is>
        static std::array<NumericParam<T>, N> _makeComponents(
            const std::array<std::string, N>& keys,
            const std::array<std::string, N>& titles,
            std::index_sequence<Is...>
        );

        static std::array<std::string, N> _defaultKeys();
        static std::array<std::string, N> _defaultTitles();

        template <typename Func>
        void _forEachParam(Func&& func) const;
        template <typename Func>
        void _forEachParam(Func&& func);

       public:
        /**
         * @brief Construct with custom component keys and titles
         *
         * @param key             JSON key for the group
         * @param title           Human-readable title
         * @param description     Description
         * @param componentKeys   One key per component
         * @param componentTitles One title per component
         */
        VecNParam(
            std::string                key,
            std::string                title,
            std::string                description,
            std::array<std::string, N> componentKeys,
            std::array<std::string, N> componentTitles
        );

        /**
         * @brief Construct with default component keys ("x","y","z","w",…)
         *
         * @param key         JSON key for the group
         * @param title       Human-readable title
         * @param description Description
         */
        VecNParam(std::string key, std::string title, std::string description);

        // ── Generic indexed access ───────────────────────────────────────────

        [[nodiscard]] NumericParam<T>&       component(std::size_t i);
        [[nodiscard]] const NumericParam<T>& component(std::size_t i) const;

        [[nodiscard]] const T& get(std::size_t i) const;
        [[nodiscard]] std::expected<void, ParamError> set(
            std::size_t i,
            const T&    value
        );

        // ── Named sub-param accessors ────────────────────────────────────────

        [[nodiscard]] NumericParam<T>&       x();
        [[nodiscard]] const NumericParam<T>& x() const;

        [[nodiscard]] NumericParam<T>&       y();
        [[nodiscard]] const NumericParam<T>& y() const;

        [[nodiscard]] NumericParam<T>&       z() requires(N >= 3);
        [[nodiscard]] const NumericParam<T>& z() const requires(N >= 3);

        [[nodiscard]] NumericParam<T>&       w() requires(N >= 4);
        [[nodiscard]] const NumericParam<T>& w() const requires(N >= 4);

        // ── Named value getters ──────────────────────────────────────────────

        [[nodiscard]] const T& getX() const;
        [[nodiscard]] const T& getY() const;
        [[nodiscard]] const T& getZ() const requires(N >= 3);
        [[nodiscard]] const T& getW() const requires(N >= 4);

        // ── Named value setters ──────────────────────────────────────────────

        [[nodiscard]] std::expected<void, ParamError> setX(const T& v);
        [[nodiscard]] std::expected<void, ParamError> setY(const T& v);
        [[nodiscard]] std::expected<void, ParamError> setZ(const T& v)
            requires(N >= 3);
        [[nodiscard]] std::expected<void, ParamError> setW(const T& v)
            requires(N >= 4);
    };

    // ── Convenience aliases ──────────────────────────────────────────────────

    template <typename T>
    using Vec2Param = VecNParam<T, 2>;

    template <typename T>
    using Vec3Param = VecNParam<T, 3>;

    template <typename T>
    using Vec4Param = VecNParam<T, 4>;

}   // namespace settings

#ifndef __SETTINGS__PARAMS__VECN_PARAM_TPP__
#include "vecn_param.tpp"
#endif   // __SETTINGS__PARAMS__VECN_PARAM_TPP__

#endif   // __SETTINGS__PARAMS__VECN_PARAM_HPP__
