#ifndef __SETTINGS__PARAM_CONTAINER_HPP__
#define __SETTINGS__PARAM_CONTAINER_HPP__

#include <string>

namespace settings
{
    /**
     * @brief Base class for settings containers that hold parameters
     *
     * This class provides a common interface for settings containers that hold
     * parameters. It defines a pure virtual function to get the parameters as a
     * tuple, which derived classes must implement.
     */
    class ParamContainer
    {
       private:
        std::string _key;
        std::string _title;
        std::string _description;

       public:
        ParamContainer(
            std::string key,
            std::string title,
            std::string description
        );

        virtual ~ParamContainer() = default;

        [[nodiscard]] const std::string& getKey() const;
        [[nodiscard]] const std::string& getTitle() const;
        [[nodiscard]] const std::string& getDescription() const;
    };
}   // namespace settings

#endif   // __SETTINGS__PARAM_CONTAINER_HPP__