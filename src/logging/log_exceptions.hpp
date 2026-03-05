#ifndef __LOGGING__LOG_EXCEPTIONS_HPP__
#define __LOGGING__LOG_EXCEPTIONS_HPP__

#include "exceptions/base.hpp"

namespace logging
{
    /**
     * @brief Base class for all logging-related exceptions
     *
     */
    class LogException : public MolarTrackerException
    {
       public:
        explicit LogException(const std::string& message);

        [[nodiscard]] const char* what() const noexcept override;
    };

}   // namespace logging

#endif   // __LOGGING__LOG_EXCEPTIONS_HPP__