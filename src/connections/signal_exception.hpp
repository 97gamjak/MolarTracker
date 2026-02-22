#ifndef __CONNECTIONS__SIGNAL_EXCEPTION_HPP__
#define __CONNECTIONS__SIGNAL_EXCEPTION_HPP__

#include "exceptions/base.hpp"

/**
 * @brief Exception class for Signal-related errors, this can be thrown when
 * there is an error related to the Signal class, such as a null callback
 * function or an overflow of the subscriber ID counter
 */
class SignalException : public MolarTrackerException
{
   public:
    explicit SignalException(const std::string& message);

    [[nodiscard]] const char* what() const noexcept override;
};

#endif   // __CONNECTIONS__SIGNAL_EXCEPTION_HPP__