#ifndef __UI__COMMANDS__COMMAND_HPP__
#define __UI__COMMANDS__COMMAND_HPP__

#include <expected>
#include <string>

#include "command_error.hpp"

namespace ui
{

    /**
     * @brief Interface for undoable/redoable commands
     *
     */
    class ICommand
    {
       public:
        virtual ~ICommand() = default;

        /**
         * @brief Undo the command
         *
         * @return std::expected<void, CommandErrorPtr>
         */
        virtual std::expected<void, CommandErrorPtr> undo() = 0;

        /**
         * @brief Redo the command
         *
         * @return std::expected<void, CommandErrorPtr>
         */
        virtual std::expected<void, CommandErrorPtr> redo() = 0;

        /**
         * @brief Get the label of the command, this is used to display the
         * command in the undo/redo stack, this should be a user-friendly
         * string that describes the command, for example "Add Profile" or
         * "Delete Profile"
         *
         * @return std::string
         */
        [[nodiscard]] virtual std::string getLabel() const = 0;

        template <typename CommandType, typename... Args>
        static std::expected<std::unique_ptr<ICommand>, CommandErrorPtr> makeAndDo(
            Args&&... args
        )
        {
            auto command =
                std::make_unique<CommandType>(std::forward<Args>(args)...);

            auto result = command->redo();
            if (!result)
                return std::unexpected(std::move(result).error());

            return std::move(command);
        }
    };

}   // namespace ui

#endif   // __UI__COMMANDS__COMMAND_HPP__