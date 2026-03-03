#ifndef __UI__COMMANDS__UPDATE_DEBUG_FLAGS_COMMAND_HPP__
#define __UI__COMMANDS__UPDATE_DEBUG_FLAGS_COMMAND_HPP__

#include <expected>

#include "command.hpp"
#include "logging/log_manager.hpp"

namespace ui
{
    /**
     * @brief A command for updating debug flag categories
     *
     */
    class UpdateDebugFlagsCommand : public ICommand
    {
       private:
        /// The old categories before the update
        logging::LogCategories _oldCategories;
        /// The new categories after the update
        logging::LogCategories _categories;

       public:
        explicit UpdateDebugFlagsCommand(logging::LogCategories categories);
        ~UpdateDebugFlagsCommand() override = default;

        std::expected<void, CommandErrorPtr> undo() override;
        std::expected<void, CommandErrorPtr> redo() override;

        std::string getLabel() const override;
    };

}   // namespace ui

#endif   // __UI__COMMANDS__UPDATE_DEBUG_FLAGS_COMMAND_HPP__