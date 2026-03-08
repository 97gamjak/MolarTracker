#ifndef __SETTINGS__PARAMS__NUMERIC_VEC_PARAM_HPP__
#define __SETTINGS__PARAMS__NUMERIC_VEC_PARAM_HPP__

#include <vector>

#include "numeric_param.hpp"

namespace settings
{
    class NumericVecParamSchema
    {
       public:
        static constexpr const char* KEY_KEY         = "key";
        static constexpr const char* TITLE_KEY       = "title";
        static constexpr const char* DESCRIPTION_KEY = "description";
        static constexpr const char* PARAMS_VEC_KEY  = "params";
    };

    /**
     * @brief A parameter class for a vector of numeric values
     *
     * @tparam T The type of the numeric values in the vector, this should be a
     * numeric type such as int, float, double, etc.
     * @tparam N The size of the vector, this should be a positive integer
     */
    template <typename T, std::size_t N>
    class NumericVecParam
    {
       private:
        using Schema = NumericVecParamSchema;

       private:
        std::string _key;
        std::string _title;
        std::string _description;

        std::vector<NumericParam<T>> _params;

       public:
        NumericVecParam(
            const std::string& keyPrefix,
            const std::string& titlePrefix,
            const std::string& descriptionPrefix
        );

        void setDefaults(const std::vector<T>& defaultValues);

        void commit();

        static void fromJson(
            const nlohmann::json&  jsonData,
            NumericVecParam<T, N>& param
        );
        [[nodiscard]] nlohmann::json toJson() const;

        [[nodiscard]] std::string getKey() const;
    };

    template <typename T>
    using Numeric2DParam = NumericVecParam<T, 2>;

}   // namespace settings

#endif   // __SETTINGS__PARAMS__NUMERIC_VEC_PARAM_HPP__
