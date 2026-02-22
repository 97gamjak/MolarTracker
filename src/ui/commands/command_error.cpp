#include "command_error.hpp"

namespace ui
{
    /**
     * @brief Construct a new Command Error:: Command Error object
     *
     * @param type Type of the command error
     */
    CommandError::CommandError(Type type) : _type{type} {}

    /**
     * @brief Get the error message
     *
     * @return std::string
     */
    std::string CommandError::getMessage() const
    {
        switch (_type)
        {
            case Type::InvalidCommand:
                return "The command is invalid.";
            case Type::NothingToRedo:
                return "There is nothing to redo.";
            case Type::NothingToUndo:
                return "There is nothing to undo.";
        }
        return "An unknown error occurred.";
    }

    /**
     * @brief Get the error code as a string
     *
     * @return std::string
     */
    std::string CommandError::getCodeStr() const
    {
        switch (_type)
        {
            case Type::InvalidCommand:
                return "InvalidCommand";
            case Type::NothingToRedo:
                return "NothingToRedo";
            case Type::NothingToUndo:
                return "NothingToUndo";
        }
        return "UnknownError";
    }

    /**
     * @brief Create a CommandErrorPtr for NothingToUndo error
     *
     * @return CommandErrorPtr
     */
    CommandErrorPtr CommandError::makeNothingToUndoErrorPtr()
    {
        return std::make_shared<CommandError>(Type::NothingToUndo);
    }

    /**
     * @brief Create a CommandErrorPtr for NothingToRedo error
     *
     * @return CommandErrorPtr
     */
    CommandErrorPtr CommandError::makeNothingToRedoErrorPtr()
    {
        return std::make_shared<CommandError>(Type::NothingToRedo);
    }

    /**
     * @brief Create a CommandErrorPtr for InvalidCommand error
     *
     * @return CommandErrorPtr
     */
    CommandErrorPtr CommandError::makeInvalidCommandErrorPtr()
    {
        return std::make_shared<CommandError>(Type::InvalidCommand);
    }

}   // namespace ui