#ifndef __UI__COMMANDS__COMMAND_TPP__
#define __UI__COMMANDS__COMMAND_TPP__

#include "command.hpp"

namespace ui
{
    /**
     * @brief Helper function to create a command and execute it immediately,
     * this is used to simplify the process of creating and executing commands,
     * as it allows us to create a command and execute it in a single step, and
     * it also handles the case where the command execution fails, by returning
     * the error instead of the command, this is useful for cases where we want
     * to execute a command and handle the error in the same step, without
     * having to create the command first and then execute it, which would
     * require multiple steps and error handling in between.
     *
     * @tparam CommandType
     * @tparam Args
     * @param args
     * @return std::expected<std::unique_ptr<ICommand>, CommandErrorPtr>
     */
    template <typename CommandType, typename... Args>
    std::expected<std::unique_ptr<ICommand>, CommandErrorPtr> ICommand::
        makeAndDo(Args&&... args)
    {
        auto command =
            std::make_unique<CommandType>(std::forward<Args>(args)...);

        auto result = command->redo();
        if (!result)
            return std::unexpected(std::move(result).error());

        // cppcheck-suppress returnStdMoveLocal
        return std::move(command);
    }
}   // namespace ui

#endif   // __UI__COMMANDS__COMMAND_TPP__