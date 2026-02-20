#ifndef __SETTINGS__PARAMS__PARAM_ERROR_HPP__
#define __SETTINGS__PARAMS__PARAM_ERROR_HPP__

#include <string>

#include "exceptions/base.hpp"

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
        /// The error message describing the parameter error
        std::string _message;

       public:
        explicit ParamError(std::string message);

        [[nodiscard]] const std::string& getMessage() const;
    };

    /**
     * @brief Exception class for setting parameters, this is used to represent
     * an exception that occurs when setting a parameter value, this is used in
     * the set function of the ParamMixin class when an error occurs that cannot
     * be represented by the ParamError class, such as a critical error that
     * should be handled by the caller
     */
    class ParamException : public MolarTrackerException
    {
       public:
        explicit ParamException(std::string message);

        const char* what() const noexcept override;
    };

}   // namespace settings

#endif   // __SETTINGS__PARAMS__PARAM_ERROR_HPP__