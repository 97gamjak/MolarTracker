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
        virtual std::string getLabel() const = 0;
    };

}   // namespace ui

#endif   // __UI__COMMANDS__COMMAND_HPP__