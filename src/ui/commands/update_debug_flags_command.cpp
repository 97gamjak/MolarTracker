#include "update_debug_flags_command.hpp"

#include "logging/log_manager.hpp"

namespace ui
{
    UpdateDebugFlagsCommand::UpdateDebugFlagsCommand(
        const LogCategoryMap& categories
    )
        : _oldCategories(LogManager::getInstance().getCategories()),
          _categories(categories)
    {
    }

    std::expected<void, CommandErrorPtr> UpdateDebugFlagsCommand::undo()
    {
        for (const auto& [category, level] : _oldCategories)
            LogManager::getInstance().changeLogLevel(category, level);

        return {};
    }

    std::expected<void, CommandErrorPtr> UpdateDebugFlagsCommand::redo()
    {
        for (const auto& [category, level] : _categories)
            LogManager::getInstance().changeLogLevel(category, level);

        return {};
    }

    std::string UpdateDebugFlagsCommand::getLabel() const
    {
        return "Updating debug flag categories";
    }

}   // namespace ui