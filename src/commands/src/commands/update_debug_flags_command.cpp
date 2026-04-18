#include "commands/update_debug_flags_command.hpp"

#include "logging/log_manager.hpp"

namespace cmd
{
    /**
     * @brief Construct a new Update Debug Flags Command:: Update Debug Flags
     * Command object
     *
     * @param categories The new debug flag categories to set
     */
    UpdateDebugFlagsCommand::UpdateDebugFlagsCommand(
        logging::LogCategories categories
    )
        : _oldCategories(logging::LogManager::getInstance().getCategories()),
          _categories(std::move(categories))
    {
    }

    /**
     * @brief Undo the update debug flags command
     *
     * @return std::expected<void, CommandErrorPtr> Result of the operation
     */
    std::expected<void, CommandErrorPtr> UpdateDebugFlagsCommand::undo()
    {
        for (const auto& category : _oldCategories.getCategories())
        {
            logging::LogManager::getInstance().changeLogLevel(
                category,
                category.getLogLevel()
            );
        }

        return {};
    }

    /**
     * @brief Redo the update debug flags command
     *
     * @return std::expected<void, CommandErrorPtr> Result of the operation
     */
    std::expected<void, CommandErrorPtr> UpdateDebugFlagsCommand::redo()
    {
        for (const auto& category : _categories.getCategories())
        {
            logging::LogManager::getInstance().changeLogLevel(
                category,
                category.getLogLevel()
            );
        }

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

}   // namespace cmd