#ifndef __APP__SRC__APP__STORE__PROFILE__EXCEPTION_HPP__
#define __APP__SRC__APP__STORE__PROFILE__EXCEPTION_HPP__

#include "exceptions/base.hpp"

namespace app
{
    /**
     * @brief Exception class for errors related to the ProfileStore. Inherits
     * from MolarTrackerException and provides a custom error message prefix for
     * profile store errors.
     *
     */
    class ProfileStoreException : public MolarTrackerException
    {
       public:
        explicit ProfileStoreException(std::string message);

        [[nodiscard]] const char* what() const noexcept override;
    };
}   // namespace app

#endif   // __APP__SRC__APP__STORE__PROFILE__EXCEPTION_HPP__