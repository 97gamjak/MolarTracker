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

    /**
     * @brief Concrete implementation of ICommandError
     *
     */
    class CommandError : public ICommandError
    {
       public:
        enum class Type
        {
            InvalidCommand,
            NothingToRedo,
            NothingToUndo
        };

        CommandError(Type type) : _type(type) {}

        std::string getMessage() const override
        {
            switch (_type)
            {
                case Type::InvalidCommand:
                    return "Invalid command";
                case Type::NothingToRedo:
                    return "Nothing to redo";
                case Type::NothingToUndo:
                    return "Nothing to undo";
            }
            return "Unknown error";
        }

        std::string getCodeStr() const override
        {
            switch (_type)
            {
                case Type::InvalidCommand:
                    return "invalid_command";
                case Type::NothingToRedo:
                    return "nothing_to_redo";
                case Type::NothingToUndo:
                    return "nothing_to_undo";
            }
            return "unknown_error";
        }

        static std::unique_ptr<CommandError> makeNothingToUndoErrorPtr()
        {
            return std::make_unique<CommandError>(Type::NothingToUndo);
        }

        static std::unique_ptr<CommandError> makeNothingToRedoErrorPtr()
        {
            return std::make_unique<CommandError>(Type::NothingToRedo);
        }

        static std::unique_ptr<CommandError> makeInvalidCommandErrorPtr()
        {
            return std::make_unique<CommandError>(Type::InvalidCommand);
        }

       private:
        Type _type;
    };

}   // namespace ui

#endif   // __UI__COMMANDS__COMMAND_ERROR_HPP__
