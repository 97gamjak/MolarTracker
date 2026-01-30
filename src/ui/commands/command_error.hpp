#ifndef __UI__COMMANDS__COMMAND_ERROR_HPP__
#define __UI__COMMANDS__COMMAND_ERROR_HPP__

#include <memory>
#include <string>

namespace ui
{
    /**
     * @brief Interface for command errors
     *
     */
    class ICommandError
    {
       public:
        virtual ~ICommandError()               = default;
        virtual std::string getMessage() const = 0;
        virtual std::string getCodeStr() const = 0;
    };

    using CommandErrorPtr = std::unique_ptr<ICommandError>;

}   // namespace ui

#endif   // __UI__COMMANDS__COMMAND_ERROR_HPP__
