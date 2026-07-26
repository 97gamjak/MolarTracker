#ifndef __SETTINGS__INCLUDE__SETTINGS__PARAMS__VEC_PARAM_HPP__
#define __SETTINGS__INCLUDE__SETTINGS__PARAMS__VEC_PARAM_HPP__

#include "error/param_error.hpp"
#include "param_core.hpp"

namespace settings
{

    template <typename Value>
    class VecParam
    {
       private:
        /// Key for the parameter
        std::string _key;

        /// Title for the parameter
        std::string _title;

        /// Description for the parameter
        std::string _description;

        /// Vector of numeric parameters
        std::vector<ParamCore<Value>> _params;

        using ValidatorFunc = std::function<ParamResult<void>(
            const ParamCore<Value>&,
            const std::vector<ParamCore<Value>>&
        )>;

        std::vector<ValidatorFunc> _validators;

       public:
        VecParam(
            const std::string& key,
            const std::string& title,
            const std::string& description,
            bool               uniqueValues = false
        );

        [[nodiscard]] const std::string& getKey() const;
        [[nodiscard]] const std::string& getTitle() const;
        [[nodiscard]] const std::string& getDescription() const;

        void addParam(const ParamCore<Value>& value);
        void set(std::size_t index, const Value& value);
        void addValidator(ValidatorFunc validator);
    };

}   // namespace settings

#endif   // __SETTINGS__INCLUDE__SETTINGS__PARAMS__VEC_PARAM_HPP__