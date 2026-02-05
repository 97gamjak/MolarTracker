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

        virtual std::expected<void, CommandErrorPtr> undo() = 0;
        virtual std::expected<void, CommandErrorPtr> redo() = 0;

        virtual std::string getLabel() const = 0;
    };

}   // namespace ui

#endif   // __UI__COMMANDS__COMMAND_HPP__