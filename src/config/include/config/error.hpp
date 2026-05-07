#ifndef __CONFIG__INCLUDE__CONFIG__ERROR_HPP__
#define __CONFIG__INCLUDE__CONFIG__ERROR_HPP__

#include <mstd/type_traits.hpp>
#include <mstd/type_traits/enum_traits.hpp>
#include <string>

/**
 * @brief An error object
 *
 * @tparam EnumType
 */
template <typename EnumType>
requires mstd::has_enum_meta<EnumType>
class Error
{
   protected:
    /// The type of the error type
    using ErrorType = EnumType;

   private:
    /// The type of the error
    EnumType _type;
    /// The error message
    std::string _message;

   public:
    explicit Error(EnumType type, std::string message);

    [[nodiscard]] EnumType           getType() const;
    [[nodiscard]] const std::string& getMessage() const;
    [[nodiscard]] std::string        toString() const;

    template <typename NewEnumType>
    [[nodiscard]]
    static Error<NewEnumType> toError(const Error& error, NewEnumType newType);
};

#ifndef __CONFIG__INCLUDE__CONFIG__DETAILS__ERROR_TPP__
#include "details/error.tpp"
#endif

#endif   // __CONFIG__INCLUDE__CONFIG__ERROR_HPP__