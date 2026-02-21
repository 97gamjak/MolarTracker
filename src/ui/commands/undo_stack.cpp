#include "undo_stack.hpp"

#define __LOG_CATEGORY__ LogCategory::undoStack
#include "logging/log_macros.hpp"

namespace ui
{

    /**
     * @brief Push a new command onto the undo stack
     *
     * @param command The command to be pushed onto the stack
     */
    void UndoStack::push(Commands command)
    {
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
     * @return std::expected<void, CommandErrorPtr> Result of the operation
     */
    std::expected<void, CommandErrorPtr> UndoStack::undo()
    {
        if (!canUndo())
            return std::unexpected(CommandError::makeNothingToUndoErrorPtr());

        if (!_commands[_cursor - 1].isUndoRedoDisabled())
        {
            --_cursor;
            auto result = _commands[_cursor - 1].undo();

            if (!result)
                return std::unexpected(std::move(result).error());
        }

        emit changed();

        return {};
    }

    /**
     * @brief Redo the next command
     *
     * @return std::expected<void, CommandErrorPtr> Result of the operation
     */
    std::expected<void, CommandErrorPtr> UndoStack::redo()
    {
        if (!canRedo())
            return std::unexpected(CommandError::makeNothingToRedoErrorPtr());

        if (!_commands[_cursor].isUndoRedoDisabled())
        {
            // Policy: if redo fails, keep cursor unchanged and do not advance.
            auto result = _commands[_cursor].redo();
            if (!result)
                return std::unexpected(std::move(result).error());

            ++_cursor;
        }

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

        const auto& combinedLabels = _commands[_cursor - 1].getCombinedLabels();
        const auto& label          = _commands[_cursor - 1].getLabel();

        if (_commands[_cursor - 1].isUndoRedoDisabled())
        {
            LOG_WARNING(
                std::format(
                    "Command '{}' cannot be undone because undo/redo is "
                    "disabled for this command.",
                    label
                )
            );
            return "Undo (Disabled)";
        }

        LOG_INFO(
            std::format(
                "Undoing: {} with the following subcommands {}",
                label,
                combinedLabels
            )
        );

        return label;
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

        const auto& combinedLabels = _commands[_cursor].getCombinedLabels();
        const auto& label          = _commands[_cursor].getLabel();

        if (_commands[_cursor].isUndoRedoDisabled())
        {
            LOG_WARNING(
                std::format(
                    "Command '{}' cannot be redone because undo/redo is "
                    "disabled for this command.",
                    label
                )
            );
            return "Redo (Disabled)";
        }

        LOG_INFO(
            std::format(
                "Redoing: {} with the following subcommands {}",
                label,
                combinedLabels
            )
        );

        return combinedLabels;
    }

}   // namespace ui