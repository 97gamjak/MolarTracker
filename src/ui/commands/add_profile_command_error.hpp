#ifndef __UI__COMMANDS__ADD_PROFILE_COMMAND_ERROR_HPP__
#define __UI__COMMANDS__ADD_PROFILE_COMMAND_ERROR_HPP__

#include <cstdint>
#include <string>

#include "command_error.hpp"
#include "mstd/enum.hpp"

#define ADD_PROFILE_COMMAND_ERROR_CODE(X) \
    X(NameAlreadyExists)                  \
    X(InvalidEmail)                       \
    X(ProfileNotFound)                    \
    X(UnknownError)

MSTD_ENUM(
    AddProfileCommandErrorCode,
    std::size_t,
    ADD_PROFILE_COMMAND_ERROR_CODE
)

namespace ui
{
    /**
     * @brief A class for representing errors when adding a profile
     *
     * @notes inherits from ICommandError interface
     *
     */
    class AddProfileCommandError final : public ICommandError
    {
       private:
        std::string                _message;
        AddProfileCommandErrorCode _code;

       public:
        AddProfileCommandError(
            const std::string&         message,
            AddProfileCommandErrorCode code
        );

        std::string                getMessage() const override;
        std::string                getCodeStr() const override;
        AddProfileCommandErrorCode getCode() const;
    };

}   // namespace ui

#endif   // __UI__COMMANDS__ADD_PROFILE_COMMAND_ERROR_HPP__
