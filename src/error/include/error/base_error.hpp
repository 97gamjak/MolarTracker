#ifndef __ERROR__INCLUDE__ERROR__BASE_ERROR_HPP__
#define __ERROR__INCLUDE__ERROR__BASE_ERROR_HPP__

#include <expected>
#include <mstd/enum.hpp>
#include <mstd/error.hpp>
#include <mstd/type_traits.hpp>
#include <optional>
#include <source_location>
#include <string>
#include <type_traits>
#include <vector>

template <typename T>
concept IsError = requires(T type) {
    typename T::ErrorType;
    typename T::ErrorTypeMeta;
};

/**
 * @brief A wrapper for an error type, providing a uniform interface for
 * accessing error messages.
 *
 * @tparam Error The error type to wrap, which must satisfy the IsError
 * concept.
 */
template <typename ErrorType>
struct ErrorWrapper
{
    /// The wrapped error type, which must satisfy the IsError concept.
    ErrorType error;

    [[nodiscard]]
    const std::string& getMessage() const;
};

// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define GENERIC_ERRORS(X) X(AssertionFailed) X(NotYetImplemented)

template <typename E>
concept ErrorType = std::is_enum_v<E> && mstd::has_enum_meta<E> && requires {
    { E::AssertionFailed } -> std::convertible_to<E>;
    { E::NotYetImplemented } -> std::convertible_to<E>;
};

/**
 * @brief An error object
 *
 * @tparam EnumType
 */
template <ErrorType EnumType>
class Error
{
   public:
    /// The type of the error type
    using ErrorType = EnumType;

    /// The meta type of the error type, used for reflection and obtaining
    /// metadata about the EnumType.
    using ErrorTypeMeta = mstd::enum_meta_t<ErrorType>;

    template <typename>
    friend struct ErrorWrapper;

   private:
    /// The type of the error
    EnumType _type;
    /// The error message
    std::string _message;
    /// A vector of sub-errors, allowing for hierarchical error representation
    /// and tracking of related errors.
    std::vector<Error> _subErrors;

#ifndef NDEBUG
    /// The source location where the error was created, used for debugging
    std::source_location _location;
#endif

   public:
    explicit Error(
        EnumType                   type,
        std::optional<std::string> message   = std::nullopt,
        std::vector<Error>         subErrors = {}
#ifndef NDEBUG
        ,
        std::source_location location = std::source_location::current()
#endif
    );

    virtual ~Error() = default;

    /// @cond DOXYGEN_IGNORE
    [[nodiscard]] EnumType getType() const;
    /// @endcond
    [[nodiscard]] std::string getTypeStr() const;
    /// @cond DOXYGEN_IGNORE
    [[nodiscard]] virtual std::string toString() const;
    /// @endcond

    [[nodiscard]]
    const std::vector<Error>& getSubErrors() const;

    Error convert(
        const EnumType&                   newType,
        const std::optional<std::string>& newMessage  = std::nullopt,
        bool                              addSubError = true
    ) const;

   protected:
    /// @cond DOXYGEN_IGNORE
    [[nodiscard]] const std::string& getMessage() const;
    /// @endcond

   private:
    bool operator==(const Error& other) const;
};

/**
 * @brief Converts an error of type OldErrorType to an error of type
 * NewErrorType, preserving the error message and sub-errors.
 *
 * @tparam OldErrorType The original error type, which must satisfy the
 * IsError concept.
 * @tparam NewErrorType The target error type, which must satisfy the
 * IsError concept.
 */
template <IsError OldErrorType, IsError NewErrorType>
struct FromError
{
    /**
     * @brief Converts an error of type OldErrorType to an error of type
     * NewErrorType, preserving the error message and sub-errors.
     *
     * @param error The original error of type OldErrorType to convert.
     * @param newType The new error type to convert to, of type
     * NewErrorType::ErrorType.
     * @param newMessage An optional new error message to use for the
     * converted error. If not provided, the original error message will be
     * used.
     * @return NewErrorType A new error of type NewErrorType with the same
     * message and sub-errors as the original error.
     */
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

    // NOLINTBEGIN(google-explicit-constructor, hicpp-explicit-conversions)
    // cppcheck-suppress noExplicitConstructor
    Result(const E& error);
    // NOLINTEND(google-explicit-constructor, hicpp-explicit-conversions)

    /**
     * @brief Creates a Result object representing a successful operation with
     * no value.
     *
     * @return Result
     */
    static Result ok()
    requires std::is_void_v<T>
    {
        return {};
    }
};

template <typename R>
concept ResultLike = requires(const R& result) {
    { static_cast<bool>(result) } -> std::same_as<bool>;
    { result.error() };
};

// bool + explicit error — unchanged
template <IsError E>
[[nodiscard]]
std::optional<E> mt_assert_check(bool cond, E err)
{
    if (!cond)
        return err;
    return std::nullopt;
}

// any Result-like type — works for MTResult, FinanceResult, future ones
template <ResultLike R>
[[nodiscard]]
auto mt_assert_check(const R& result)
    -> std::optional<std::decay_t<decltype(result.error())>>
{
    if (!result)
        return result.error();
    return std::nullopt;
}

template <typename EnumType>
requires ErrorType<EnumType>
[[nodiscard]]
auto mt_assert_check(bool cond, const EnumType& errorType)
    -> std::optional<Error<EnumType>>
{
    if (!cond)
        return Error<EnumType>{errorType, "Assertion failed"};
    return std::nullopt;
}

// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define MT_ASSERT(...)                               \
    if (auto _mt_err = mt_assert_check(__VA_ARGS__)) \
        return *_mt_err;

#ifndef __ERROR__INCLUDE__ERROR__BASE_ERROR_TPP__
#include "base_error.tpp"
#endif

#endif   // __ERROR__INCLUDE__ERROR__BASE_ERROR_HPP__