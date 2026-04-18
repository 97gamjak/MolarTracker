#include "commands/command_error.hpp"

namespace cmd
{
    /**
     * @brief Construct a new Command Error:: Command Error object
     *
     * @param type Type of the command error
     */
    CommandError::CommandError(CommandErrorType type) : _type{type} {}

    /**
     * @brief Get the error message
     *
     * @return std::string
     */
    std::string CommandError::getMessage() const
    {
        switch (_type)
        {
            case CommandErrorType::InvalidCommand:
                return "The command is invalid.";
            case CommandErrorType::NothingToRedo:
                return "There is nothing to redo.";
            case CommandErrorType::NothingToUndo:
                return "There is nothing to undo.";
            case CommandErrorType::UndoNotImplemented:
                return "Undo is not implemented for this command.";
            case CommandErrorType::RedoNotImplemented:
                return "Redo is not implemented for this command.";
            case CommandErrorType::UndoNotSupported:
                return "Undo is not supported for this command.";
            case CommandErrorType::RedoNotSupported:
                return "Redo is not supported for this command.";
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
        return CommandErrorTypeMeta::toString(_type);
    }

    /**
     * @brief Create a CommandErrorPtr for NothingToUndo error
     *
     * @return CommandErrorPtr
     */
    CommandErrorPtr CommandError::makeNothingToUndoErrorPtr()
    {
        return std::make_shared<CommandError>(CommandErrorType::NothingToUndo);
    }

    /**
     * @brief Create a CommandErrorPtr for NothingToRedo error
     *
     * @return CommandErrorPtr
     */
    CommandErrorPtr CommandError::makeNothingToRedoErrorPtr()
    {
        return std::make_shared<CommandError>(CommandErrorType::NothingToRedo);
    }

    /**
     * @brief Create a CommandErrorPtr for InvalidCommand error
     *
     * @return CommandErrorPtr
     */
    CommandErrorPtr CommandError::makeInvalidCommandErrorPtr()
    {
        return std::make_shared<CommandError>(CommandErrorType::InvalidCommand);
    }

    /**
     * @brief Create a CommandErrorPtr for UndoNotImplemented error
     *
     * @return CommandErrorPtr
     */
    CommandErrorPtr CommandError::makeUndoNotImplementedErrorPtr()
    {
        return std::make_shared<CommandError>(
            CommandErrorType::UndoNotImplemented
        );
    }

    /**
     * @brief Create a CommandErrorPtr for RedoNotImplemented error
     *
     * @return CommandErrorPtr
     */
    CommandErrorPtr CommandError::makeRedoNotImplementedErrorPtr()
    {
        return std::make_shared<CommandError>(
            CommandErrorType::RedoNotImplemented
        );
    }

    /**
     * @brief Create a CommandErrorPtr for UndoNotSupported error
     *
     * @return CommandErrorPtr
     */
    CommandErrorPtr CommandError::makeUndoNotSupportedErrorPtr()
    {
        return std::make_shared<CommandError>(CommandErrorType::UndoNotSupported
        );
    }

    /**
     * @brief Create a CommandErrorPtr for RedoNotSupported error
     *
     * @return CommandErrorPtr
     */
    CommandErrorPtr CommandError::makeRedoNotSupportedErrorPtr()
    {
        return std::make_shared<CommandError>(CommandErrorType::RedoNotSupported
        );
    }

}   // namespace cmd