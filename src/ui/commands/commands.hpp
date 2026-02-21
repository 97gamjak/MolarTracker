#ifndef __UI__COMMANDS__COMMANDS_HPP__
#define __UI__COMMANDS__COMMANDS_HPP__

#include <memory>
#include <vector>

#include "command.hpp"

namespace ui
{
    class Commands
    {
       private:
        /// Vector of unique pointers to ICommand objects that are part of this
        /// command, this allows us to group multiple commands together as a
        /// single command in the undo stack.
        std::vector<std::unique_ptr<ICommand>> _commands;

        std::string _label{};

        bool _disableUndoRedo = false;

       public:
        explicit Commands() = default;
        explicit Commands(std::string label);

        template <typename ICommandType, typename... Args>
        static std::expected<Commands, CommandErrorPtr> makeAndDo(
            Args&&... args
        )
        {
            auto result =
                ICommand::makeAndDo<ICommandType>(std::forward<Args>(args)...);

            if (!result)
                return std::unexpected(std::move(result).error());

            return Commands::_fromCommand(std::move(result).value());
        }

        Commands& operator<<(std::expected<Commands, CommandErrorPtr>&& result);
        Commands& operator<<(Commands&& commands);
        Commands& operator<<(Commands& commands) = delete;

        [[nodiscard]] std::expected<void, CommandErrorPtr> undo();
        [[nodiscard]] std::expected<void, CommandErrorPtr> redo();

        [[nodiscard]] std::string              getLabel() const;
        [[nodiscard]] std::vector<std::string> getLabels() const;
        [[nodiscard]] std::string              getCombinedLabels() const;

        [[nodiscard]] bool        empty() const;
        [[nodiscard]] std::size_t size() const;

        [[nodiscard]] bool isUndoRedoDisabled() const;
        void               disableUndoRedo(bool disable = true);

       private:
        static Commands _fromCommand(std::unique_ptr<ICommand>&& command);
    };

}   // namespace ui

#endif   // __UI__COMMANDS__COMMANDS_HPP__