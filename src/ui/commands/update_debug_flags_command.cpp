#include "update_debug_flags_command.hpp"

#include "logging/log_manager.hpp"

namespace ui
{
    /**
     * @brief Construct a new Update Debug Flags Command:: Update Debug Flags
     * Command object
     *
     * @param categories The new debug flag categories to set
     */
    UpdateDebugFlagsCommand::UpdateDebugFlagsCommand(
        const LogCategoryMap& categories
    )
        : _oldCategories(LogManager::getInstance().getCategories()),
          _categories(categories)
    {
    }

    /**
     * @brief Undo the update debug flags command
     *
     * @return std::expected<void, CommandErrorPtr> Result of the operation
     */
    std::expected<void, CommandErrorPtr> UpdateDebugFlagsCommand::undo()
    {
        for (const auto& [category, level] : _oldCategories)
            LogManager::getInstance().changeLogLevel(category, level);

        return {};
    }

    /**
     * @brief Redo the update debug flags command
     *
     * @return std::expected<void, CommandErrorPtr> Result of the operation
     */
    std::expected<void, CommandErrorPtr> UpdateDebugFlagsCommand::redo()
    {
        for (const auto& [category, level] : _categories)
            LogManager::getInstance().changeLogLevel(category, level);

        return {};
    }

    /**
     * @brief Get the label of the command
     *
     * @return std::string The label of the command
     */
    std::string UpdateDebugFlagsCommand::getLabel() const
    {
        return "Updating debug flag categories";
    }

}   // namespace ui