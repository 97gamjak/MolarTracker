#ifndef __SETTINGS__PARAMS_CORE_HPP__
#define __SETTINGS__PARAMS_CORE_HPP__

#include <optional>
#include <string>

namespace settings
{

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
        T    _value{};
        T    _baseLine{};
        bool _hasBaseLine = false;

        std::optional<T> _defaultValue = std::nullopt;

        std::string _key;
        std::string _title;
        std::string _description;

       public:
        ParamCore() = delete;
        ParamCore(const std::string& key, const std::string& title);

        [[nodiscard]] const T& get() const;
        void                   set(const T& value);

        void commit();

        [[nodiscard]] bool isDirty() const;

        [[nodiscard]] const std::optional<T>& getDefault() const;
        void setDefault(const std::optional<T>& defaultValue);

        [[nodiscard]] const std::string& getKey() const;
        [[nodiscard]] const std::string& getTitle() const;

        [[nodiscard]] const std::string& getDescription() const;
        void setDescription(const std::string& description);

       private:
        static bool _equals(const T& a, const T& b);
    };

}   // namespace settings

#ifndef __SETTINGS__PARAMS_CORE_TPP__
#include "params_core.tpp"
#endif   // __SETTINGS__PARAMS_CORE_TPP__

#endif   // __SETTINGS__PARAMS_CORE_HPP__