#ifndef __APP__STORE__PROFILE__EXCEPTION_HPP__
#define __APP__STORE__PROFILE__EXCEPTION_HPP__

#include "exceptions/base.hpp"

namespace app
{
    class ProfileStoreException : public MolarTrackerException
    {
       public:
        explicit ProfileStoreException(std::string message);

        [[nodiscard]] const char* what() const noexcept override;
    };
}   // namespace app

#endif   // __APP__STORE__PROFILE__EXCEPTION_HPP__