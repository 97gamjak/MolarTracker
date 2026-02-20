#ifndef __SETTINGS__PARAMS__PARAM_CORE_HPP__
#define __SETTINGS__PARAMS__PARAM_CORE_HPP__

#include <optional>
#include <string>
#include <unordered_map>

#include "connections/connection.hpp"
#include "nlohmann/json.hpp"

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
     * @brief Core class for a setting parameter, this is a template class that
     * can be used for any type of setting parameter
     *
     * @tparam T
     */
    template <typename T>
    class ParamCore
    {
       private:
        /// Type alias for the templated change callback function, this is a
        /// function pointer that takes a pointer to the user data and the new
        /// value of the parameter, this is used to notify subscribers when the
        /// parameter value changes
        template <typename U>
        using ChangedFnBase = void (*)(void*, const U&);

        /// Type alias for the change callback function for the parameter value,
        /// this is a function pointer that takes a pointer to the user data and
        /// the new value of the parameter, this is used to notify subscribers
        /// when the parameter value changes
        using ChangedFn = ChangedFnBase<T>;

        /// Type alias for the change callback function for the optional
        /// parameter value, this is a function pointer that takes a pointer to
        /// the user data and the new optional value of the parameter, this is
        /// used to notify subscribers when the parameter value changes, this is
        /// useful for parameters that can be unset (optional)
        using ChangedFnOptional = ChangedFnBase<std::optional<T>>;

        /**
         * @brief Subscriber struct for a setting parameter, this struct
         * contains the callback function and the user data for a subscriber,
         * this is used to notify subscribers of changes in the parameter value
         */
        template <typename U>
        struct Subscriber
        {
            /// The callback function to call when the parameter value changes
            ChangedFnBase<U> fn{};
            /// A user-defined pointer that will be passed to the callback
            void* user{};
        };

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

        /// A counter for generating unique subscriber IDs, this is used to
        /// assign IDs to subscribers when they subscribe to changes in the
        /// parameter value, this allows for subscribers to be uniquely
        /// identified and unsubscribed when needed
        size_t _idCounter = 0;

        /// The subscribers for changes in the parameter value, this is a map of
        /// subscriber IDs to Subscriber structs, this allows for multiple
        /// subscribers to be notified of changes in the parameter value
        std::unordered_map<size_t, Subscriber<T>> _subscribers;

        /// The subscribers for changes in the optional parameter value, this is
        /// a map of subscriber IDs to Subscriber structs, this allows for
        /// multiple subscribers to be notified of changes in the parameter
        /// value, this is useful for parameters that can be unset (optional)
        std::unordered_map<size_t, Subscriber<std::optional<T>>>
            _optionalSubscribers;

       public:
        ParamCore() = delete;
        ParamCore(const std::string& key, const std::string& title);
        ParamCore(
            const std::string& key,
            const std::string& title,
            const std::string& description
        );

        [[nodiscard]] const std::optional<T>& getOptional() const;
        [[nodiscard]] const T&                get() const;
        void                                  set(const T& value);
        void                                  unset();

        Connection subscribeToOptional(ChangedFnOptional fn, void* user);
        Connection subscribe(ChangedFn fn, void* user);

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

        [[nodiscard]] nlohmann::json toJson() const;
        static void fromJson(const nlohmann::json& j, ParamCore<T>& param);

       private:
        [[nodiscard]] static bool _equals(const T& a, const T& b);

        void _notifySubscribers();
        template <typename U>
        static void _disconnect(void* owner, std::size_t id);
        template <typename U>
        Connection _subscribe(ChangedFnBase<U> fn, void* user);
    };

}   // namespace settings

#ifndef __SETTINGS__PARAM_CORE_TPP__
#include "param_core.tpp"
#endif   // __SETTINGS__PARAM_CORE_TPP__

#endif   // __SETTINGS__PARAMS__PARAM_CORE_HPP__