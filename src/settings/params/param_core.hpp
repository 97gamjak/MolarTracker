#ifndef __SETTINGS__PARAM_CORE_HPP__
#define __SETTINGS__PARAM_CORE_HPP__

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
        static constexpr const char* KEY_KEY         = "key";
        static constexpr const char* TITLE_KEY       = "title";
        static constexpr const char* DESCRIPTION_KEY = "description";
        static constexpr const char* VALUE_KEY       = "value";
        static constexpr const char* DEFAULT_KEY     = "default";
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
        using ChangedFn = void (*)(void*, const std::optional<T>&);

        /**
         * @brief Subscriber struct for a setting parameter, this struct
         * contains the callback function and the user data for a subscriber,
         * this is used to notify subscribers of changes in the parameter value
         */
        struct Subscriber
        {
            ChangedFn fn{};
            void*     user{};
        };

       private:
        using Schema = ParamCoreSchema;

        std::unordered_map<size_t, Subscriber> _subscribers;

        std::optional<T> _value        = std::nullopt;
        std::optional<T> _baseLine     = std::nullopt;
        std::optional<T> _defaultValue = std::nullopt;

        std::string _key;
        std::string _title;
        std::string _description;

        bool   _isRebootRequired = false;
        size_t _idCounter        = 0;

       public:
        ParamCore() = delete;
        ParamCore(const std::string& key, const std::string& title);
        ParamCore(
            const std::string& key,
            const std::string& title,
            const std::string& description
        );

        [[nodiscard]] const std::optional<T>& get() const;
        void                                  set(const T& value);
        void                                  unset();

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

        static void _disconnect(void* owner, std::size_t id);

        void _notifySubscribers();
    };

}   // namespace settings

#ifndef __SETTINGS__PARAM_CORE_TPP__
#include "param_core.tpp"
#endif   // __SETTINGS__PARAM_CORE_TPP__

#endif   // __SETTINGS__PARAM_CORE_HPP__