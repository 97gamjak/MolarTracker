#ifndef __SETTINGS__INCLUDE__SETTINGS__PARAMS__NUMERIC_VEC_PARAM_HPP__
#define __SETTINGS__INCLUDE__SETTINGS__PARAMS__NUMERIC_VEC_PARAM_HPP__

#include <vector>

#include "connections/observable.hpp"
#include "numeric_param.hpp"

namespace settings
{
    /**
     * @brief Schema for the JSON representation of a NumericVecParam
     *
     */
    class NumericVecParamSchema
    {
       public:
        /// key for the parameter key in JSON
        static constexpr const char* KEY_KEY = "key";

        /// key for the parameter title in JSON
        static constexpr const char* TITLE_KEY = "title";

        /// key for the parameter description in JSON
        static constexpr const char* DESCRIPTION_KEY = "description";

        /// key for the parameter values in JSON
        static constexpr const char* PARAMS_VEC_KEY = "params";
    };

    /**
     * @brief A parameter class for a vector of numeric values
     *
     * @tparam T The type of the numeric values in the vector, this should be a
     * numeric type such as int, float, double, etc.
     * @tparam N The size of the vector, this should be a positive integer
     */
    template <typename T, std::size_t N>
    requires(N > 1)
    class NumericVecParam : public Observable<OnDirtyChanged>
    {
       public:
        /// The size of the vector
        constexpr static std::size_t size = N;

        /// Alias for the type of the numeric values in the vector
        using value_type = T;

       private:
        /// Base class for the observable functionality
        using Base = Observable<OnDirtyChanged>;

        /// Schema for the JSON representation of the parameter
        using Schema = NumericVecParamSchema;

       private:
        /// Key for the parameter
        std::string _key;

        /// Title for the parameter
        std::string _title;

        /// Description for the parameter
        std::string _description;

        /// Vector of numeric parameters
        std::vector<NumericParam<T>> _params;

       public:
        NumericVecParam(
            const std::string& keyPrefix,
            const std::string& titlePrefix,
            const std::string& descriptionPrefix
        );

        void setDefaults(const std::vector<T>& defaultValues);
        void setDefaults(const std::pair<T, T>& defaultValues)
        requires(N == 2);
        void setMinValues(const std::vector<T>& minValues);
        void setMinValues(const std::pair<T, T>& minValues)
        requires(N == 2);

        const T&               get(std::size_t index) const;
        const NumericParam<T>& getParam(std::size_t index) const;
        NumericParam<T>&       getParam(std::size_t index);
        [[nodiscard]] std::expected<void, ParamError> set(
            std::size_t index,
            const T&    value
        );

        void commit();

        static void fromJson(
            const nlohmann::json&  jsonData,
            NumericVecParam<T, N>& param
        );
        [[nodiscard]] nlohmann::json toJson() const;

        [[nodiscard]] const std::string& getKey() const;

        std::vector<Connection> subscribeToDirty(
            OnDirtyChanged::func func,
            void*                user
        );

        [[nodiscard]] bool isDirty() const;

        [[nodiscard]] std::string toString() const;
        [[nodiscard]] std::string getDescription() const;
        [[nodiscard]] std::string getTitle() const;

        const T& x() const
        requires(N <= 3);

        const T& y() const
        requires(N <= 3);

        const T& z() const
        requires(N == 3);
    };

    template <typename T>
    using Numeric2DParam = NumericVecParam<T, 2>;

}   // namespace settings

#ifndef __SETTINGS__INCLUDE__SETTINGS__PARAMS__NUMERIC_VEC_PARAM_TPP__
#include "numeric_vec_param.tpp"
#endif

#endif   // __SETTINGS__INCLUDE__SETTINGS__PARAMS__NUMERIC_VEC_PARAM_HPP__
