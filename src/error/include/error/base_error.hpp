#ifndef __ERROR__INCLUDE__ERROR__BASE_ERROR_HPP__
#define __ERROR__INCLUDE__ERROR__BASE_ERROR_HPP__

#include <expected>
#include <mstd/enum.hpp>
#include <mstd/error.hpp>
#include <mstd/type_traits.hpp>
#include <optional>
#include <string>
#include <vector>

template <typename T>
concept IsError = requires(T type) {
    typename T::ErrorType;
    typename T::ErrorTypeMeta;
};

template <typename ErrorType>
struct ErrorWrapper
{
    ErrorType error;

    [[nodiscard]]
    const std::string& getMessage() const;
};

/**
 * @brief An error object
 *
 * @tparam EnumType
 */
template <typename EnumType>
requires mstd::has_enum_meta<EnumType>
class Error
{
   public:
    /// The type of the error type
    using ErrorType = EnumType;

    /// The meta type of the error type, used for reflection and obtaining
    /// metadata about the EnumType.
    using ErrorTypeMeta = mstd::enum_meta_t<ErrorType>;

    friend struct ErrorWrapper<Error>;

   private:
    /// The type of the error
    EnumType _type;
    /// The error message
    std::string _message;
    /// A vector of sub-errors, allowing for hierarchical error representation
    /// and tracking of related errors.
    std::vector<Error> _subErrors;

   public:
    explicit Error(
        EnumType                   type,
        std::optional<std::string> message   = std::nullopt,
        std::vector<Error>         subErrors = {}
    );

    virtual ~Error() = default;

    [[nodiscard]] EnumType            getType() const;
    [[nodiscard]] std::string         getTypeStr() const;
    [[nodiscard]] virtual std::string toString() const;

    [[nodiscard]]
    const std::vector<Error>& getSubErrors() const;

    Error convert(
        const EnumType&                   newType,
        const std::optional<std::string>& newMessage = std::nullopt
    ) const;

   private:
    [[nodiscard]]
    const std::string& getMessage() const;
};

template <IsError OldErrorType, IsError NewErrorType>
struct FromError
{
    static NewErrorType apply(
        const OldErrorType&               error,
        const NewErrorType::ErrorType&    newType,
        const std::optional<std::string>& newMessage = std::nullopt
    )
    {
        std::vector<NewErrorType> subErrors;
        for (const auto& subError : error.getSubErrors())
        {
            subErrors.push_back(
                FromError<OldErrorType, NewErrorType>::apply(
                    dynamic_cast<const OldErrorType&>(subError),
                    newType,
                    newMessage
                )
            );
        }

        return NewErrorType(
            newType,
            newMessage.value_or(ErrorWrapper{error}.getMessage()),
            subErrors
        );
    }
};

/**
 * @brief A result type that can either hold a value of type T or an error of
 * type E. This is a wrapper around std::expected that provides additional
 * functionality for working with errors.
 *
 * @tparam T The type of the value.
 * @tparam E The type of the error, which must satisfy the IsError concept.
 */
template <typename T, IsError E>
class Result : public std::expected<T, E>
{
   public:
    using std::expected<T, E>::expected;

    // NOLINTNEXTLINE(google-explicit-constructor, hicpp-explicit-conversions)
    Result(const E& error);
};

#ifndef __ERROR__INCLUDE__ERROR__BASE_ERROR_TPP__
#include "base_error.tpp"
#endif

#endif   // __ERROR__INCLUDE__ERROR__BASE_ERROR_HPP__