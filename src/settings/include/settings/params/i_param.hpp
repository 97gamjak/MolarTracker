#ifndef __SETTINGS__INCLUDE__SETTINGS__PARAMS__I_PARAM_HPP__
#define __SETTINGS__INCLUDE__SETTINGS__PARAMS__I_PARAM_HPP__

#include <nlohmann/json.hpp>
#include <string>

#include "config/signal_tags.hpp"

class Connections;   // Forward declaration

namespace settings
{
    /**
     * @brief Interface for a parameter, this is used to define the common
     * functionality that all parameters must implement, this allows for
     * polymorphic behavior when dealing with different types of parameters,
     * such as BoolParam, StringParam, NumericParam, etc.
     *
     */
    class IParam
    {
       public:
        virtual ~IParam() = default;

        /**
         * @brief Get the key of the parameter, this is used to identify the
         * parameter in a map or list of parameters, it should be unique within
         * the context of the parameters it is used in.
         *
         * @return const std::string& The key of the parameter.
         */
        [[nodiscard]] virtual const std::string& getKey() const = 0;

        /**
         * @brief Get the title of the parameter, this is used to display a
         * human-readable name for the parameter in the UI, it should be
         * descriptive and concise.
         *
         * @return const std::string& The title of the parameter.
         */
        [[nodiscard]] virtual const std::string& getTitle() const = 0;

        /**
         * @brief Get the description of the parameter, this is used to provide
         * additional information about the parameter in the UI, it should be
         * clear and informative.
         *
         * @return const std::string& The description of the parameter.
         */
        [[nodiscard]] virtual const std::string& getDescription() const = 0;

        /**
         * @brief Get a string representation of the parameter, this is used for
         * logging and debugging purposes, it should provide a clear and
         * concise summary of the parameter's state.
         *
         * @return std::string The string representation of the parameter.
         */
        [[nodiscard]] virtual std::string toString() const = 0;

        /**
         * @brief Commit the current value of the parameter, this is used to
         * save the current state of the parameter, it should be called when
         * the user confirms their changes.
         */
        virtual void commit() = 0;

        /**
         * @brief Reset the parameter to its default value, this is used to
         * revert any changes made to the parameter, it should be called when
         * the user wants to discard their changes.
         */
        virtual void resetToDefault() = 0;

        /**
         * @brief Check if the parameter has been modified since the last commit
         * or reset operation, this is used to determine if the parameter's
         * state has changed and if it needs to be saved.
         *
         * @return true If the parameter has been modified.
         * @return false If the parameter has not been modified.
         */
        [[nodiscard]] virtual bool isDirty() const = 0;

        /**
         * @brief Serialize the parameter to JSON, this is used to save the
         * parameter's state to a file or send it over a network, it should
         * produce a valid JSON representation of the parameter.
         *
         * @return nlohmann::json The JSON representation of the parameter.
         */
        [[nodiscard]] virtual nlohmann::json toJson() const = 0;

        /**
         * @brief Subscribe to changes in the parameter's dirty state, this is
         * used to notify observers when the parameter's state has changed,
         * allowing for reactive behavior in the UI or other components.
         *
         * @param func The callback function to be called when the dirty state
         * changes.
         * @param user A user-defined pointer that will be passed to the
         * callback function.
         * @return Connections An object representing the subscription, which
         * can be used to manage the connection.
         */
        [[nodiscard]]
        virtual Connections subscribeToDirty(
            OnDirtyChanged::func func,
            void*                user
        ) = 0;
    };

}   // namespace settings

#endif   // __SETTINGS__INCLUDE__SETTINGS__PARAMS__I_PARAM_HPP__