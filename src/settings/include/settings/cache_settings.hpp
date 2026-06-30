#ifndef __SETTINGS__INCLUDE__SETTINGS__CACHE_SETTINGS_HPP__
#define __SETTINGS__INCLUDE__SETTINGS__CACHE_SETTINGS_HPP__

#include <cstddef>

#include "settings/params/numeric_param.hpp"
#include "settings/params/param_container.hpp"
#include "settings/params/param_container_mixin.hpp"
namespace settings
{
    /**
     * @brief Schema for the cache settings. This defines the keys and
     * descriptions for the cache settings parameters.
     *
     */
    class CacheSettingsSchema
    {
       public:
        /// The key for the cache settings container
        static constexpr const char* CACHE_SETTINGS_KEY = "cache";
        /// The title for the cache settings container
        static constexpr const char* CACHE_SETTINGS_TITLE = "Cache Settings";
        /// The description for the cache settings container
        static constexpr const char* CACHE_SETTINGS_DESC =
            "Settings related to caching behavior";

        /// The key for the global maximum cache size setting
        static constexpr const char* GLOBAL_MAX_CACHE_SIZE_KEY =
            "globalMaxCacheSize";
        /// The title for the global maximum cache size setting
        static constexpr const char* GLOBAL_MAX_CACHE_SIZE_TITLE =
            "Global Max Cache Size";
        /// The description for the global maximum cache size setting
        static constexpr const char* GLOBAL_MAX_CACHE_SIZE_DESC =
            "The maximum size of the global cache, in number of elements. This "
            "setting controls the total amount of elements that can be used "
            "for caching data across the application. Setting this value too "
            "low may result in frequent cache evictions, while setting it too "
            "high may lead to excessive memory usage.";

        // number of elements
        /// The default value for the global maximum cache size setting
        static constexpr std::size_t GLOBAL_MAX_CACHE_SIZE_DEFAULT = 1e6;
        /// The minimum value for the global maximum cache size setting
        static constexpr std::size_t GLOBAL_MAX_CACHE_SIZE_MIN = 1e3;
        /// The maximum value for the global maximum cache size setting
        static constexpr std::size_t GLOBAL_MAX_CACHE_SIZE_MAX = 1e12;
    };

    /**
     * @brief Cache related settings management
     */
    class CacheSettings : public ParamContainerMixin<CacheSettings>
    {
       private:
        /// type alias for CacheSettingsSchema
        using Schema = CacheSettingsSchema;
        /// Grant ParamContainerMixin access to private members of CacheSettings
        friend ParamContainerMixin<CacheSettings>;

        /// The core container for the cache settings parameters
        ParamContainer _core{
            Schema::CACHE_SETTINGS_KEY,
            Schema::CACHE_SETTINGS_TITLE,
            Schema::CACHE_SETTINGS_DESC
        };

        /// The maximum size of the global cache, in number of elements.
        NumericParam<std::size_t> _globalMaxCacheSize{
            Schema::GLOBAL_MAX_CACHE_SIZE_KEY,
            Schema::GLOBAL_MAX_CACHE_SIZE_TITLE,
            Schema::GLOBAL_MAX_CACHE_SIZE_DESC
        };

       public:
        CacheSettings();

        [[nodiscard]]
        NumericParam<std::size_t>& getGlobalMaxCacheSize();

        template <typename Func>
        void forEachParam(Func&& func) const;
        template <typename Func>
        void forEachParam(Func&& func);
    };

}   // namespace settings

#ifndef __SETTINGS__INCLUDE__SETTINGS__CACHE_SETTINGS_TPP__
#include "cache_settings.tpp"
#endif   // __SETTINGS__INCLUDE__SETTINGS__CACHE_SETTINGS_TPP__

#endif   // __SETTINGS__INCLUDE__SETTINGS__CACHE_SETTINGS_HPP__