#ifndef __UI__COMMANDS__COMMAND_ERROR_HPP__
#define __UI__COMMANDS__COMMAND_ERROR_HPP__

namespace ui
{
    class ICommandError
    {
       public:
        virtual ~ICommandError()            = default;
        virtual std::string message() const = 0;
        virtual std::string code() const    = 0;
    };

}   // namespace ui

using CommandErrorPtr = std::unique_ptr<ICommandError>;

#endif   // __UI__COMMANDS__COMMAND_ERROR_HPP__
