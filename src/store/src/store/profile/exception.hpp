#ifndef __STORE__SRC__STORE__PROFILE__EXCEPTION_HPP__
#define __STORE__SRC__STORE__PROFILE__EXCEPTION_HPP__

#include "exceptions/base.hpp"

namespace store
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
        explicit ProfileStoreException(const std::string& message);

        [[nodiscard]] const char* what() const noexcept override;
    };
}   // namespace store

#endif   // __STORE__SRC__STORE__PROFILE__EXCEPTION_HPP__