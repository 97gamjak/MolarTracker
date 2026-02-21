#include "commands.hpp"

#include <expected>
#include <utility>

namespace ui
{
    /**
     * @brief Construct a new Commands:: Commands object
     *
     * @param label
     */
    Commands::Commands(std::string label) : _label(std::move(label)) {}

    /**
     * @brief Create a Commands object from a single command, this is used to
     * simplify the process of creating a Commands object from a single command,
     * as it allows us to create a Commands object from a single command in a
     * single step, and it also handles the case where the command execution
     * fails, by returning the error instead of the Commands object, this is
     * useful for cases where we want to execute a command and handle the error
     * in the same step, without having to create the command first and then
     * execute it, which would require multiple steps and error handling in
     * between.
     *
     * @param command The command to create the Commands object from
     * @return Commands A Commands object containing the given command, or an
     * error if the command execution failed
     */
    Commands Commands::_fromCommand(std::unique_ptr<ICommand>&& command)
    {
        Commands commands;
        if (command)
            commands._commands.push_back(std::move(command));
        return commands;
    }

    /**
     * @brief operator<< for adding a command to the Commands object, this
     * allows us to add a command to the Commands object using the << operator,
     * which is more convenient and readable than calling a method to add the
     * command, and it also allows us to chain multiple commands together using
     * the << operator, which is useful for cases where we want to execute
     * multiple commands as a single undoable/redoable action.
     *
     * @param result The result of creating a command, if the command creation
     * was successful, the command will be added to the Commands object, if the
     * command creation failed, the error will be ignored and the Commands
     * object will remain unchanged, this is because the error should have
     * already been handled at the point where the command was created, and it
     * is not the responsibility of the Commands class to handle errors from
     * command creation, this is a design choice that allows us to keep the
     * Commands class focused on managing commands and their execution, rather
     * than handling errors from command creation, which can be complex and
     * varied, and it allows us to handle those errors in a more appropriate
     * place in the code where the command is being created and executed.
     *
     * @return Commands& This Commands object with the given command added to
     * it, or this Commands object unchanged if the command creation failed.
     */
    Commands& Commands::operator<<(
        std::expected<Commands, CommandErrorPtr>&& result
    )
    {
        if (result)
            return *this << std::move(result).value();

        // TODO: maybe throw an exception here, as this indicates a logic error
        // in the code where the command was created, and silently ignoring it
        // might lead to unexpected behavior. This should be investigated
        // further to determine the best way to handle this case, and whether it
        // should be considered a fatal error or if it can be safely ignored.
        return *this;
    }

    /**
     * @brief operator<< for adding another Commands object to this Commands
     * object, this allows us to add multiple commands to the Commands object
     * using the << operator, which is more convenient and readable than calling
     * a method to add the commands, and it also allows us to chain multiple
     * commands together using the << operator, which is useful for cases where
     * we want to execute multiple commands as a single undoable/redoable
     * action.
     *
     * @param commands The Commands object containing the commands to add to
     * this Commands object, if the given Commands object is empty, this method
     * will do nothing and return this Commands object unchanged, if the given
     * Commands object is not empty, its commands will be added to this Commands
     * object, and its label will be combined with this Commands object's label,
     * and its disableUndoRedo flag will be combined with this Commands object's
     * disableUndoRedo flag, and the given Commands object's commands will be
     * cleared after being added to this Commands object, this is because once
     * the commands have been added to this Commands object, they should not be
     * kept in the given Commands object anymore, as they are now owned by this
     * Commands object, and keeping them in the given Commands object could lead
     * to confusion and bugs if someone tries to use the given Commands object
     * after its commands have been added to another Commands object.
     *
     * @return Commands& This Commands object with the given Commands object's
     * commands added to it, or this Commands object unchanged if the given
     * Commands object is empty.
     */
    Commands& Commands::operator<<(Commands&& commands)
    {
        if (commands.empty())
            return *this;

        _commands.reserve(size() + commands.size());
        for (auto& command : commands._commands)
            _commands.push_back(std::move(command));

        if (_label.empty())
            _label = commands._label;
        else if (!commands._label.empty())
            _label += " > " + commands._label;

        _disableUndoRedo = _disableUndoRedo || commands._disableUndoRedo;

        commands._commands.clear();
        return *this;
    }

    /**
     * @brief Clear all sub-commands from this Commands object
     *
     */
    void Commands::clearSubCommands() { _commands.clear(); }

    /**
     * @brief Undo all commands in this Commands object, if any command fails to
     * undo, the undo process will stop and the error will be returned, and any
     * commands that were successfully undone before the failure will be redone
     * to restore the state to what it was before the undo process started, this
     * is a design choice that allows us to maintain consistency and avoid
     * leaving the application in an inconsistent state if an undo operation
     * fails, as it ensures that either all commands are successfully undone or
     * none of them are, and it allows us to handle errors from undo operations
     * in a more appropriate place in the code where the undo operation is being
     * performed, rather than handling those errors in the Commands class, which
     * is focused on managing commands and their execution rather than error
     * handling.
     *
     * @return std::expected<void, CommandErrorPtr> Result of the operation
     */
    std::expected<void, CommandErrorPtr> Commands::undo()
    {
        for (std::size_t i = _commands.size(); i > 0; --i)
        {
            auto& command = _commands[i - 1];
            auto  result  = command->undo();
            if (result)
                continue;

            for (std::size_t j = i; j < _commands.size(); ++j)
            {
                auto& _command = _commands[j];
                auto  _redo    = _command->redo();
                if (!_redo)
                {
                    return std::unexpected(std::move(_redo).error());
                }
            }
            return std::unexpected(std::move(result).error());
        }
        return {};
    }

    /**
     * @brief Redo all commands in this Commands object, if any command fails to
     * redo, the redo process will stop and the error will be returned, and any
     * commands that were successfully redone before the failure will be undone
     * to restore the state to what it was before the redo process started, this
     * is a design choice that allows us to maintain consistency and avoid
     * leaving the application in an inconsistent state if a redo operation
     * fails, as it ensures that either all commands are successfully redone or
     * none of them are, and it allows us to handle errors from redo operations
     * in a more appropriate place in the code where the redo operation is being
     * performed, rather than handling those errors in the Commands class, which
     * is focused on managing commands and their execution rather than error
     * handling.
     *
     * @return std::expected<void, CommandErrorPtr> Result of the operation
     */
    std::expected<void, CommandErrorPtr> Commands::redo()
    {
        for (std::size_t i = 0; i < _commands.size(); ++i)
        {
            auto result = _commands[i]->redo();
            if (!result)
            {
                for (std::size_t j = i; j > 0; --j)
                {
                    auto _undo = _commands[j - 1]->undo();
                    if (!_undo)
                    {
                        return std::unexpected(std::move(_undo).error());
                    }
                }
                return std::unexpected(std::move(result).error());
            }
        }
        return {};
    }

    /**
     * @brief Get the label of this Commands object, if this Commands object has
     * a label, it will be returned, otherwise if this Commands object has only
     * one command, the label of that command will be returned, otherwise an
     * empty string will be returned, this is a design choice that allows us to
     * have a meaningful label for a Commands object even if it does not have an
     * explicit label set, as it allows us to use the label of the single
     * command contained in this Commands object as the label for this Commands
     * object, which can be useful for cases where we want to create a Commands
     * object from a single command and we want that command's label to be used
     * as the label for the Commands object without having to explicitly set it.
     *
     * @return std::string The label of this Commands object, or the label of
     * the single command contained in this Commands object, or an empty string
     * if there are multiple commands and no explicit label is set.
     */
    std::string Commands::getLabel() const
    {
        if (_label.empty() && size() == 1)
            return _commands[0]->getLabel();

        return _label;
    }

    /**
     * @brief Get the labels of all commands contained in this Commands object,
     * this is useful for cases where we want to display the labels of all
     * commands contained in this Commands object, for example in a tooltip or a
     * log message, to provide more context about what commands are being
     * executed as part of this Commands object.
     *
     * @return std::vector<std::string> A vector containing the labels of all
     * commands contained in this Commands object.
     */
    std::vector<std::string> Commands::getLabels() const
    {
        std::vector<std::string> labels;

        for (const auto& command : _commands)
            labels.push_back(command->getLabel());

        return labels;
    }

    /**
     * @brief Get a combined label of all commands contained in this Commands
     * object, this is useful for cases where we want to display a single label
     * that represents all commands contained in this Commands object, for
     * example in an undo/redo menu item, to provide a concise description of
     * what the undo/redo action will do without having to list all individual
     * command labels.
     *
     * @return std::string A string that combines the labels of all commands
     * contained in this Commands object, separated by " > ", or an empty string
     * if there are no commands.
     */
    std::string Commands::getCombinedLabels() const
    {
        std::string combinedLabel;

        for (const auto& label : getLabels())
        {
            if (!label.empty())
            {
                if (!combinedLabel.empty())
                    combinedLabel += " > ";
                combinedLabel += label;
            }
        }

        return combinedLabel;
    }

    /**
     * @brief Check if this Commands object is empty, meaning it contains no
     * commands, this is useful for cases where we want to check if there are
     * any commands to execute before trying to execute them, or to determine if
     * a Commands object should be added to the undo stack, as we might not want
     * to add empty Commands objects to the undo stack since they do not
     * represent any action.
     *
     * @return true If this Commands object is empty and contains no commands.
     * @return false If this Commands object contains one or more commands.
     */
    bool Commands::empty() const { return _commands.empty(); }

    /**
     * @brief Get the number of commands contained in this Commands object, this
     * is useful for cases where we want to know how many commands are being
     * executed as part of this Commands object, or to determine if a Commands
     * object should be added to the undo stack based on the number of commands
     * it contains, for example we might want to only add Commands objects that
     * contain more than one command to the undo stack, as they represent more
     * complex actions that are worth undoing/redoing as a single unit.
     *
     * @return std::size_t The number of commands contained in this Commands
     * object.
     */
    std::size_t Commands::size() const { return _commands.size(); }

    /**
     * @brief Check if undo/redo is disabled for this Commands object, this is
     * useful for cases where we want to determine if a Commands object can be
     * undone or redone, for example in the undo/redo menu items, to disable
     * those menu items if the Commands object cannot be undone or redone, as
     * this provides better user feedback and prevents users from trying to undo
     * or redo actions that cannot be undone or redone.
     *
     * @return true If undo/redo is disabled for this Commands object.
     * @return false If undo/redo is enabled for this Commands object.
     */
    bool Commands::isUndoRedoDisabled() const { return _disableUndoRedo; }

    /**
     * @brief Disable or enable undo/redo for this Commands object, this is
     * useful for cases where we want to prevent a Commands object from being
     * undone or redone, for example if it represents an action that cannot be
     * safely undone or redone, or if it represents an action that is not worth
     * undoing or redoing, as this allows us to provide better user feedback and
     * prevent users from trying to undo or redo actions that should not be
     * undone or redone.
     *
     * @param disable If true, undo/redo will be disabled for this Commands
     * object, if false, undo/redo will be enabled for this Commands object.
     */
    void Commands::disableUndoRedo(bool disable) { _disableUndoRedo = disable; }

}   // namespace ui