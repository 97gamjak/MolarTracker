#ifndef __SETTINGS__PARAM_ERROR_HPP__
#define __SETTINGS__PARAM_ERROR_HPP__

#include <string>

namespace settings
{

    /**
     * @brief Error class for setting parameters, this is used to represent an
     * error that occurs when setting a parameter value, this is used in the
     * std::expected return type of the set function in the ParamMixin class
     */
    class ParamError
    {
       private:
        std::string _message;

       public:
        explicit ParamError(std::string message);

        [[nodiscard]] const std::string& getMessage();
    };

}   // namespace settings

#endif   // __SETTINGS__PARAM_ERROR_HPP__