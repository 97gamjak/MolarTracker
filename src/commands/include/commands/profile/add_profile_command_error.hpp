#ifndef __COMMANDS__INCLUDE__COMMANDS__PROFILE__ADD_PROFILE_COMMAND_ERROR_HPP__
#define __COMMANDS__INCLUDE__COMMANDS__PROFILE__ADD_PROFILE_COMMAND_ERROR_HPP__

#include <mstd/enum.hpp>
#include <string>

#include "commands/command_error.hpp"

#define ADD_PROFILE_COMMAND_ERROR_CODE(X) \
    X(NameAlreadyExists)                  \
    X(InvalidEmail)                       \
    X(ProfileNotFound)                    \
    X(UnknownError)

/**
 * @brief Enum class for error codes when adding a profile
 * This enum class defines the possible error codes that can occur when adding a
 * profile. Each error code corresponds to a specific error condition that can
 * be encountered during the profile addition process. The error codes are used
 * to identify the type of error that occurred and provide more context for
 * error handling and user feedback.
 *
 */
enum class AddProfileCommandErrorCode : std::uint8_t;   // only for doxygen

MSTD_ENUM(
    AddProfileCommandErrorCode,
    std::uint8_t,
    ADD_PROFILE_COMMAND_ERROR_CODE
);

namespace cmd
{
    /**
     * @brief A class for representing errors when adding a profile
     *
     */
    class AddProfileCommandError final : public ICommandError
    {
       private:
        /// The error message describing the error that occurred when adding a
        /// profile
        std::string _message;

        /// The error code representing the type of error that occurred when
        /// adding a profile
        AddProfileCommandErrorCode _code;

       public:
        AddProfileCommandError(
            std::string                message,
            AddProfileCommandErrorCode code
        );

        [[nodiscard]] std::string                getMessage() const override;
        [[nodiscard]] std::string                getCodeStr() const override;
        [[nodiscard]] AddProfileCommandErrorCode getCode() const;
    };

}   // namespace cmd

#endif   // __COMMANDS__INCLUDE__COMMANDS__PROFILE__ADD_PROFILE_COMMAND_ERROR_HPP__
