#include "add_profile_command_error.hpp"

namespace ui
{

    AddProfileCommandError::AddProfileCommandError(
        const std::string&         message,
        AddProfileCommandErrorCode code
    )
        : _message{message}, _code{code}
    {
    }

    std::string AddProfileCommandError::getMessage() const { return _message; }

    std::string AddProfileCommandError::getCodeStr() const
    {
        return std::string(AddProfileCommandErrorCodeMeta::name(_code));
    }

    AddProfileCommandErrorCode AddProfileCommandError::getCode() const
    {
        return _code;
    }

}   // namespace ui