#include "undo_stack.hpp"

namespace ui
{

    /**
     * @brief Execute a command and add it to the undo stack
     *
     * @param command The command to execute
     * @return true if the command was executed and added to the stack, false
     * otherwise
     */
    std::expected<void, CommandErrorPtr> UndoStack::_do(
        std::unique_ptr<ICommand> command
    )
    {
        if (!command)
            return std::unexpected(CommandError::makeInvalidCommandErrorPtr());

        auto result = command->redo();

        if (!result)
            return std::unexpected(std::move(result).error());

        if (_cursor < _commands.size())
        {
            _commands.erase(
                _commands.begin() + static_cast<std::ptrdiff_t>(_cursor),
                _commands.end()
            );
        }

        _commands.push_back(std::move(command));
        _cursor = _commands.size();

        emit changed();
        return {};
    }

    /**
     * @brief Check if there are commands to undo
     *
     * @return true if there are commands to undo, false otherwise
     */
    bool UndoStack::canUndo() const { return _cursor > 0; }

    /**
     * @brief Check if there are commands to redo
     *
     * @return true if there are commands to redo, false otherwise
     */
    bool UndoStack::canRedo() const { return _cursor < _commands.size(); }

    /**
     * @brief Undo the last command
     *
     */
    std::expected<void, CommandErrorPtr> UndoStack::undo()
    {
        if (!canUndo())
            return std::unexpected(CommandError::makeNothingToUndoErrorPtr());

        --_cursor;
        auto result = _commands[_cursor]->undo();

        if (!result)
            return std::unexpected(std::move(result).error());

        emit changed();

        return {};
    }

    /**
     * @brief Redo the next command
     *
     * @return true if the command was redone, false otherwise
     */
    std::expected<void, CommandErrorPtr> UndoStack::redo()
    {
        if (!canRedo())
            return std::unexpected(CommandError::makeNothingToRedoErrorPtr());

        // Policy: if redo fails, keep cursor unchanged and do not advance.
        auto result = _commands[_cursor]->redo();
        if (!result)
            return std::unexpected(std::move(result).error());

        ++_cursor;

        emit changed();

        return {};
    }

    /**
     * @brief Get the label of the command that can be undone
     *
     * @return std::string The label of the command that can be undone, or an
     * empty string if there are no commands to undo
     */
    std::string UndoStack::getUndoLabel() const
    {
        if (!canUndo())
            return "";

        return _commands[_cursor - 1]->getLabel();
    }

    /**
     * @brief Get the label of the command that can be redone
     *
     * @return std::string The label of the command that can be redone, or an
     * empty string if there are no commands to redo
     */
    std::string UndoStack::getRedoLabel() const
    {
        if (!canRedo())
            return "";

        return _commands[_cursor]->getLabel();
    }

}   // namespace ui