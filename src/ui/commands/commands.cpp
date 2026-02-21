#include "commands.hpp"

#include <expected>
#include <utility>

namespace ui
{
    Commands::Commands(std::string label) : _label(std::move(label)) {}

    Commands Commands::_fromCommand(std::unique_ptr<ICommand>&& command)
    {
        Commands commands;
        if (command)
            commands._commands.push_back(std::move(command));
        return commands;
    }

    Commands& Commands::operator<<(
        std::expected<Commands, CommandErrorPtr>&& result
    )
    {
        if (result)
            return *this << std::move(result).value();

        // TODO: maybe throw an exception here, as this indicates a logic error
        // in the code where the command was created, and silently ignoring it
        // might lead to unexpected behavior. This should be investigated
        // further to determine the best way to handle this case, and whether it
        // should be considered a fatal error or if it can be safely ignored.
        return *this;
    }

    Commands& Commands::operator<<(Commands&& commands)
    {
        if (commands.empty())
            return *this;

        _commands.reserve(size() + commands.size());
        for (auto& command : commands._commands)
            _commands.push_back(std::move(command));

        if (_label.empty())
            _label = commands._label;
        else if (!commands._label.empty())
            _label += " > " + commands._label;

        _disableUndoRedo = _disableUndoRedo || commands._disableUndoRedo;

        commands._commands.clear();
        return *this;
    }

    std::expected<void, CommandErrorPtr> Commands::undo()
    {
        for (std::size_t i = _commands.size(); i > 0; --i)
        {
            auto& command = _commands[i - 1];
            auto  result  = command->undo();
            if (result)
                continue;

            for (std::size_t j = i; j < _commands.size(); ++j)
            {
                auto& command = _commands[j];
                auto  redo    = command->redo();
                if (!redo)
                {
                    return std::unexpected(std::move(redo).error());
                }
            }
            return std::unexpected(std::move(result).error());
        }
        return {};
    }

    std::expected<void, CommandErrorPtr> Commands::redo()
    {
        for (auto& command : _commands)
        {
            auto result = command->redo();
            if (!result)
            {
                for (auto& command : _commands)
                {
                    auto undo = command->undo();
                    if (!undo)
                    {
                        return std::unexpected(std::move(undo).error());
                    }
                }
                return std::unexpected(std::move(result).error());
            }
        }
        return {};
    }

    std::string Commands::getLabel() const { return _label; }

    std::vector<std::string> Commands::getLabels() const
    {
        std::vector<std::string> labels;

        for (const auto& command : _commands)
            labels.push_back(command->getLabel());

        return labels;
    }

    std::string Commands::getCombinedLabels() const
    {
        std::string combinedLabel;

        for (const auto& label : getLabels())
        {
            if (!label.empty())
            {
                if (!combinedLabel.empty())
                    combinedLabel += " > ";
                combinedLabel += label;
            }
        }

        return combinedLabel;
    }

    bool Commands::empty() const { return _commands.empty(); }

    std::size_t Commands::size() const { return _commands.size(); }

    bool Commands::isUndoRedoDisabled() const { return _disableUndoRedo; }

    void Commands::disableUndoRedo(bool disable) { _disableUndoRedo = disable; }

}   // namespace ui