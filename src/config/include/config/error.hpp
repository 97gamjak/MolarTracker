#ifndef __CONFIG__INCLUDE__CONFIG__ERROR_HPP__
#define __CONFIG__INCLUDE__CONFIG__ERROR_HPP__

#include <cstdint>
#include <expected>
#include <mstd/enum.hpp>
#include <mstd/type_traits.hpp>
#include <optional>
#include <string>
#include <vector>

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

    using ErrorTypeMeta = mstd::enum_meta_t<ErrorType>;

   private:
    /// The type of the error
    EnumType _type;
    /// The error message
    std::string _message;

    std::vector<Error> _subErrors;

   public:
    explicit Error(
        EnumType                   type,
        std::optional<std::string> message = std::nullopt
    );

    [[nodiscard]] EnumType           getType() const;
    [[nodiscard]] const std::string& getMessage() const;
    [[nodiscard]] std::string        toString() const;

    template <typename NewEnumType>
    [[nodiscard]]
    static Error<NewEnumType> toError(const Error& error, NewEnumType newType);

    static Error fromError(
        const Error&               error,
        EnumType                   newType,
        std::optional<std::string> newMessage = std::nullopt
    );
};

// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define FINANCE_ERROR_TYPE_LIST(X) \
    X(StockNotFound)               \
    X(AccountNotFound)             \
    X(InvalidTransaction)          \
    X(InvalidAccount)              \
    X(InvalidStock)

MSTD_ENUM(FinanceErrorType, std::uint8_t, FINANCE_ERROR_TYPE_LIST);

using FinanceError = Error<FinanceErrorType>;

template <typename T>
concept IsError = requires(T type) {
    typename T::ErrorType;
    typename T::ErrorTypeMeta;
};

template <typename T, IsError E>
class MTResult : public std::expected<T, E>
{
   public:
    using std::expected<T, E>::expected;

    // NOLINTNEXTLINE(google-explicit-constructor, hicpp-explicit-conversions)
    MTResult(const E& error);
};

#ifndef __CONFIG__INCLUDE__CONFIG__DETAILS__ERROR_TPP__
#include "details/error.tpp"
#endif

#endif   // __CONFIG__INCLUDE__CONFIG__ERROR_HPP__