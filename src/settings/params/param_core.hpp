#ifndef __SETTINGS__PARAMS__PARAM_CORE_HPP__
#define __SETTINGS__PARAMS__PARAM_CORE_HPP__

#include <optional>
#include <string>
#include <unordered_map>

#include "connections/observable.hpp"
#include "json/optional.hpp"   // IWYU pragma: keep

namespace settings
{

    /**
     * @brief Schema for a setting parameter, this struct defines the keys for
     * the JSON representation of a setting parameter
     */
    class ParamCoreSchema
    {
       public:
        /// key for the parameter key in JSON
        static constexpr const char* KEY_KEY = "key";

        /// key for the parameter title in JSON
        static constexpr const char* TITLE_KEY = "title";

        /// key for the parameter description in JSON
        static constexpr const char* DESCRIPTION_KEY = "description";

        /// key for the parameter value in JSON
        static constexpr const char* VALUE_KEY = "value";

        /// key for the parameter default value in JSON
        static constexpr const char* DEFAULT_KEY = "default";
    };

    /**
     * @brief Tag struct for indicating that the dirty state of a parameter has
     * changed, this is used for notifying subscribers when the dirty state of a
     * parameter changes, the TagType is a boolean indicating whether the
     * parameter is dirty (has unsaved changes) or not
     *
     */
    struct DirtyChanged
    {
        /// Type alias for the type of data
        using TagType = bool;
    };

    /**
     * @brief Tag struct for indicating that the value of a parameter has
     * changed, this is used for notifying subscribers when the value of a
     * parameter changes, the TagType is the new value of the parameter
     *
     * @tparam T
     */
    template <typename T>
    struct ParamValueChanged
    {
        /// Type alias for the type of data
        using TagType = T;
    };

    /**
     * @brief Tag struct for indicating that the optional value of a parameter
     * has changed, this is used for notifying subscribers when the value of a
     * parameter changes, the TagType is the new optional value of the
     * parameter, this is useful for parameters that can be unset (optional)
     *
     * @tparam T
     */
    template <typename T>
    struct ParamOptionalChanged
    {
        /// Type alias for the type of data
        using TagType = std::optional<T>;
    };

    /**
     * @brief Core class for a setting parameter, this is a template class that
     * can be used for any type of setting parameter
     *
     * @tparam T
     */
    template <typename T>
    class ParamCore : Observable<
                          DirtyChanged,
                          ParamValueChanged<T>,
                          ParamOptionalChanged<T>>
    {
       private:
        /// Type alias for the base class
        using Base = Observable<
            DirtyChanged,
            ParamValueChanged<T>,
            ParamOptionalChanged<T>>;

        /// Type alias for the change callback function for the parameter value,
        /// this is a function pointer that takes a pointer to the user data and
        /// the new value of the parameter, this is used to notify subscribers
        /// when the parameter value changes
        using ChangedFn = void (*)(void*, const T& newValue);

        /// Type alias for the change callback function for the optional
        /// parameter value, this is a function pointer that takes a pointer to
        /// the user data and the new optional value of the parameter, this is
        /// used to notify subscribers when the parameter value changes, this is
        /// useful for parameters that can be unset (optional)
        using ChangedFnOptional =
            void (*)(void*, const std::optional<T>& newValue);

        /// Type alias for the change callback function for the dirty state of
        /// the parameter, this is a function pointer that takes a pointer to
        /// the user data and a boolean indicating whether the parameter is
        /// dirty (has unsaved changes) or not, this is used to notify
        /// subscribers when the dirty state of the parameter changes
        using DirtyChangedFn = void (*)(void*, bool isDirty);

       private:
        /// type alias for ParamCoreSchema
        using Schema = ParamCoreSchema;

        /// The current value of the parameter
        std::optional<T> _value = std::nullopt;
        /// The baseline value of the parameter, used for dirty checking
        std::optional<T> _baseLine = std::nullopt;
        /// The default value of the parameter
        std::optional<T> _defaultValue = std::nullopt;

        /// The key for the parameter
        std::string _key;
        /// The title for the parameter
        std::string _title;
        /// The description for the parameter
        std::string _description;

        /// Whether changing this parameter requires a reboot, this can be used
        /// to inform the user that they need to restart the application for
        /// changes to take effect
        bool _isRebootRequired = false;

       public:
        ParamCore() = delete;
        ParamCore(std::string key, std::string title);
        ParamCore(std::string key, std::string title, std::string description);

        [[nodiscard]] const std::optional<T>& getOptional() const;
        [[nodiscard]] const T&                get() const;
        void                                  set(const T& value);
        void                                  unset();

        Connection subscribe(ChangedFn func, void* user);
        Connection subscribeToOptional(ChangedFnOptional func, void* user);
        Connection subscribeToDirty(DirtyChangedFn func, void* user);

        void commit();

        [[nodiscard]] bool isDirty() const;

        [[nodiscard]] const std::optional<T>& getDefault() const;
        void setDefault(const std::optional<T>& defaultValue);

        [[nodiscard]] const std::string& getKey() const;
        [[nodiscard]] const std::string& getTitle() const;

        [[nodiscard]] const std::string& getDescription() const;
        void setDescription(const std::string& description);

        void               setRebootRequired(bool required);
        [[nodiscard]] bool isRebootRequired() const;

        static void fromJson(
            const nlohmann::json& jsonData,
            ParamCore<T>&         param
        );
        [[nodiscard]] nlohmann::json toJson() const;

       private:
        [[nodiscard]] static bool _equals(const T& lhs, const T& rhs);

        void _notifySubscribers();
    };

}   // namespace settings

#ifndef __SETTINGS__PARAMS__PARAM_CORE_TPP__
#include "param_core.tpp"
#endif   // __SETTINGS__PARAMS__PARAM_CORE_TPP__

#endif   // __SETTINGS__PARAMS__PARAM_CORE_HPP__