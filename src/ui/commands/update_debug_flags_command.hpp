#ifndef __UI__COMMANDS__UPDATE_DEBUG_FLAGS_COMMAND_HPP__
#define __UI__COMMANDS__UPDATE_DEBUG_FLAGS_COMMAND_HPP__

#include <expected>

#include "command.hpp"
#include "logging/logging_base.hpp"

namespace ui
{
    class UpdateDebugFlagsCommand : public ICommand
    {
       private:
        LogCategoryMap _oldCategories;
        LogCategoryMap _categories;

       public:
        UpdateDebugFlagsCommand(const LogCategoryMap& categories);
        ~UpdateDebugFlagsCommand() override = default;

        std::expected<void, CommandErrorPtr> undo() override;
        std::expected<void, CommandErrorPtr> redo() override;

        std::string getLabel() const override;
    };

}   // namespace ui

#endif   // __UI__COMMANDS__UPDATE_DEBUG_FLAGS_COMMAND_HPP__