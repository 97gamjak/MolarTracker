#ifndef __UI__COMMANDS__COMMAND_ERROR_HPP__
#define __UI__COMMANDS__COMMAND_ERROR_HPP__

#include <memory>
#include <string>

namespace ui
{
    /**
     * @brief Interface for command errors
     *
     */
    class ICommandError
    {
       public:
        virtual ~ICommandError() = default;

        /**
         * @brief Get the Message object
         *
         * @return std::string
         */
        virtual std::string getMessage() const = 0;

        /**
         * @brief Get the Code Str object
         *
         * @return std::string
         */
        virtual std::string getCodeStr() const = 0;
    };

    using CommandErrorPtr = std::unique_ptr<ICommandError>;

    /**
     * @brief Concrete implementation of ICommandError
     *
     */
    class CommandError : public ICommandError
    {
       public:
        // TODO(97gamjak): implement this via mstd::enum
        // https://97gamjak.atlassian.net/browse/MOLTRACK-110
        /**
         * @brief Enum class for command error types
         *
         */
        enum class Type
        {
            InvalidCommand,
            NothingToRedo,
            NothingToUndo
        };

       private:
        /// The type of the command error
        Type _type;

       public:
        explicit CommandError(Type type);

        std::string getMessage() const override;

        std::string getCodeStr() const override;

        static std::unique_ptr<CommandError> makeNothingToUndoErrorPtr();
        static std::unique_ptr<CommandError> makeNothingToRedoErrorPtr();
        static std::unique_ptr<CommandError> makeInvalidCommandErrorPtr();
    };

}   // namespace ui

#endif   // __UI__COMMANDS__COMMAND_ERROR_HPP__
