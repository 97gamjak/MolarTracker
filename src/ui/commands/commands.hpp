#ifndef __UI__COMMANDS__COMMANDS_HPP__
#define __UI__COMMANDS__COMMANDS_HPP__

#include <memory>
#include <vector>

#include "command.hpp"

namespace ui
{
    /**
     * @brief A class for grouping multiple ICommand objects together as a
     * single command in the undo stack, this allows us to perform multiple
     * operations as a single undoable/redoable action, for example when adding
     * a profile we might want to add the profile to the profile store, set it
     * as the active profile, and set it as the default profile, all of these
     * operations can be grouped together as a single command in the undo stack,
     * so that when the user undoes the action, all of these operations are
     * undone together, and when the user redoes the action, all of these
     * operations are redone together.
     *
     */
    class Commands
    {
       private:
        /// Vector of unique pointers to ICommand objects that are part of this
        /// command, this allows us to group multiple commands together as a
        /// single command in the undo stack.
        std::vector<std::unique_ptr<ICommand>> _commands;

        /// A label for this command
        std::string _label;

        /// Flag to indicate whether undo/redo should be disabled
        bool _disableUndoRedo = false;

       public:
        explicit Commands() = default;
        explicit Commands(std::string label);

        template <typename ICommandType, typename... Args>
        static std::expected<Commands, CommandErrorPtr> makeAndDo(
            Args&&... args
        );

        Commands& operator<<(std::expected<Commands, CommandErrorPtr>&& result);
        Commands& operator<<(Commands&& commands);
        Commands& operator<<(Commands& commands) = delete;

        void clearSubCommands();

        [[nodiscard]] std::expected<void, CommandErrorPtr> undo();
        [[nodiscard]] std::expected<void, CommandErrorPtr> redo();

        [[nodiscard]] std::string              getLabel() const;
        [[nodiscard]] std::vector<std::string> getLabels() const;
        [[nodiscard]] std::string              getCombinedLabels() const;

        [[nodiscard]] bool        empty() const;
        [[nodiscard]] std::size_t size() const;

        [[nodiscard]] bool isUndoRedoDisabled() const;
        void               disableUndoRedo();
        void               disableUndoRedo(bool disable);

       private:
        static Commands _fromCommand(std::unique_ptr<ICommand>&& command);
    };

}   // namespace ui

#ifndef __UI__COMMANDS__COMMANDS_TPP__
#include "commands.tpp"
#endif   // __UI__COMMANDS__COMMANDS_TPP__

#endif   // __UI__COMMANDS__COMMANDS_HPP__