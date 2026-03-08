#ifndef __SETTINGS__PARAMS__VEC2_PARAM_HPP__
#define __SETTINGS__PARAMS__VEC2_PARAM_HPP__

#include <expected>
#include <string>

#include "numeric_param.hpp"
#include "param_container.hpp"
#include "param_error.hpp"
#include "param_group_mixin.hpp"

namespace settings
{
    /**
     * @brief A parameter group representing a 2D value (x, y)
     *
     * Combines two NumericParam<T> into a single logical parameter. The group
     * serializes as {"x": ..., "y": ...} (or custom sub-keys) nested under its
     * own key in the parent container.
     *
     * Example:
     * @code
     * Vec2Param<float> _windowPos{
     *     "windowPos", "Window Position", "Position of the main window."};
     *
     * // Access sub-params directly
     * _windowPos.x().set(100.0f);
     * _windowPos.y().set(200.0f);
     *
     * // Poll combined dirty state
     * if (_windowPos.isDirty()) { ... }
     * @endcode
     *
     * @tparam T Numeric type (int, float, double, …)
     */
    template <typename T>
    requires(std::integral<T> || std::floating_point<T>)
    class Vec2Param : public ParamGroupMixin<Vec2Param<T>>
    {
       private:
        friend ParamGroupMixin<Vec2Param<T>>;

        /// Group metadata (key / title / description)
        ParamContainer _core;

        /// X component
        NumericParam<T> _x;

        /// Y component
        NumericParam<T> _y;

        template <typename Func>
        void _forEachParam(Func&& func) const;
        template <typename Func>
        void _forEachParam(Func&& func);

       public:
        /**
         * @brief Construct a Vec2Param
         *
         * @param key         JSON key for this group
         * @param title       Human-readable title
         * @param description Description of what this value represents
         * @param xKey        JSON key for the x component (default "x")
         * @param xTitle      Title for the x component (default "X")
         * @param yKey        JSON key for the y component (default "y")
         * @param yTitle      Title for the y component (default "Y")
         */
        Vec2Param(
            std::string key,
            std::string title,
            std::string description,
            std::string xKey   = "x",
            std::string xTitle = "X",
            std::string yKey   = "y",
            std::string yTitle = "Y"
        );

        /// Direct access to the x sub-parameter
        [[nodiscard]] NumericParam<T>&       x();
        [[nodiscard]] const NumericParam<T>& x() const;

        /// Direct access to the y sub-parameter
        [[nodiscard]] NumericParam<T>&       y();
        [[nodiscard]] const NumericParam<T>& y() const;

        /// Convenience getter for x value
        [[nodiscard]] const T& getX() const;

        /// Convenience getter for y value
        [[nodiscard]] const T& getY() const;

        /// Convenience setter for x — forwards to NumericParam<T>::set()
        [[nodiscard]] std::expected<void, ParamError> setX(const T& value);

        /// Convenience setter for y — forwards to NumericParam<T>::set()
        [[nodiscard]] std::expected<void, ParamError> setY(const T& value);
    };

}   // namespace settings

#ifndef __SETTINGS__PARAMS__VEC2_PARAM_TPP__
#include "vec2_param.tpp"
#endif   // __SETTINGS__PARAMS__VEC2_PARAM_TPP__

#endif   // __SETTINGS__PARAMS__VEC2_PARAM_HPP__
