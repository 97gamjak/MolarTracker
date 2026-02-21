#include "add_profile_command_error.hpp"

namespace ui
{

    /**
     * @brief Construct a new Add Profile Command Error:: Add Profile Command
     * Error object
     *
     * @param message
     * @param code
     */
    AddProfileCommandError::AddProfileCommandError(
        const std::string&         message,
        AddProfileCommandErrorCode code
    )
        : _message{message}, _code{code}
    {
    }

    /**
     * @brief Get the Message object
     *
     * @return std::string
     */
    std::string AddProfileCommandError::getMessage() const { return _message; }

    /**
     * @brief Get the Code Str object
     *
     * @return std::string
     */
    std::string AddProfileCommandError::getCodeStr() const
    {
        return std::string(AddProfileCommandErrorCodeMeta::name(_code));
    }

    /**
     * @brief Get the Code object
     *
     * @return AddProfileCommandErrorCode
     */
    AddProfileCommandErrorCode AddProfileCommandError::getCode() const
    {
        return _code;
    }

}   // namespace ui