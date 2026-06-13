#ifndef __UTILS__INCLUDE__UTILS__RESULT__ERROR_HPP__
#define __UTILS__INCLUDE__UTILS__RESULT__ERROR_HPP__

#include <cstdint>
#include <format>
#include <mstd/enum.hpp>
#include <string>
#include <type_traits>
#include <variant>

#include "result.hpp"

// ─── Per-domain error codes
// ───────────────────────────────────────────────────
//
// Each layer defines its own enum. Codes that make sense at multiple layers
// (e.g. NotFound) are duplicated intentionally — Repo::NotFound and
// Store::NotFound are semantically distinct and dispatch separately in
// visitors.
//
// To add a new domain:
//   1. Add an enum below.
//   2. Add it to err::Kind.
//   3. Add domain_name / code_name overloads.
//   4. Add is_not_found() arm in Error::is_not_found() if needed.
//

namespace err
{

// NOLINTBEGIN(cppcoreguidelines-macro-usage)
#define DB_CODES(X)        \
    X(NotFound)            \
    X(ConstraintViolation) \
    X(QueryFailed)         \
    X(ConnectionFailed)

    MSTD_ENUM(DbError, std::uint8_t, DB_CODES)

#define REPO_CODES(X) \
    X(NotFound)       \
    X(InvalidState)   \
    X(DuplicateKey)

    MSTD_ENUM(RepoError, std::uint8_t, REPO_CODES)

#define STORE_CODES(X) \
    X(NotFound)        \
    X(Conflict)        \
    X(InvalidState)

    MSTD_ENUM(StoreError, std::uint8_t, STORE_CODES)

#define SERVICE_CODES(X) \
    X(NotFound)          \
    X(InvalidState)      \
    X(Unauthorized)

    MSTD_ENUM(ServiceError, std::uint8_t, SERVICE_CODES)

#define INPUT_CODES(X) \
    X(NotFound)        \
    X(MissingField)    \
    X(InvalidValue)    \
    X(OutOfRange)

    MSTD_ENUM(InputError, std::uint8_t, INPUT_CODES)
    // NOLINTEND(cppcoreguidelines-macro-usage)

    using Kind =
        std::variant<DbError, RepoError, StoreError, ServiceError, InputError>;

    // ── domain_name ──────────────────────────────────────────────────────────

    template <mstd::has_enum_meta T>
    [[nodiscard]]
    constexpr std::string_view domain_name(T /*unused*/)
    {
        using Meta = mstd::enum_meta_t<T>;
        return Meta::EnumNameStr;
    }

    // ── code_name
    // ────────────────────────────────────────────────────────────

    template <mstd::has_enum_meta T>
    [[nodiscard]]
    constexpr std::string_view code_name(T entry)
    {
        using Meta = mstd::enum_meta_t<T>;
        return Meta::name(entry);
    }

    // ── KindFormatter
    // ────────────────────────────────────────────────────────

    /**
     * @brief Named visitor struct that formats an err::Kind as
     * "Domain::Code".
     *
     * Follows the project's named-visitor-struct convention; avoids
     * overloaded lambdas. Use with std::visit:
     *
     *   std::string s = std::visit(err::KindFormatter{}, kind);
     */
    struct KindFormatter
    {
        /**
         * @brief Format an err::Kind as "Domain::Code".
         *
         * @param code
         * @return std::string
         */
        [[nodiscard]] std::string operator()(auto code) const
        {
            return std::format("{}::{}", domain_name(code), code_name(code));
        }
    };

}   // namespace err

// ─── Error
// ────────────────────────────────────────────────────────────────────

/**
 * @brief Unified application error carrying a domain-specific kind and a
 * human-readable message.
 *
 * Static factories make construction at each layer self-documenting:
 *   return Err{Error::error(err::Repo::NotFound, "Profile not found")};
 */
struct Error
{
    /// Domain and code of the failure
    err::Kind _kind;
    /// Human-readable description of the failure
    std::string _message;

    /**
     * @brief Static factory for database errors, e.g. Result<T,
     * Error>{Err{Error::db(err::Db::NotFound, "User not found")}}
     *
     * @tparam E
     * @param code
     * @param msg
     * @return Error
     */
    template <mstd::has_enum_meta E>
    [[nodiscard]] static Error error(E code, const std::string& msg)
    {
        return {._kind = code, ._message = msg};
    }

    /**
     * @brief get the error kind, which is a variant of all domain-specific
     * error enums.
     *
     * @return const err::Kind&
     */
    [[nodiscard]] const err::Kind& kind() const { return _kind; }

    /**
     * @brief Get the human-readable error message.
     *
     * @return const std::string&
     */
    [[nodiscard]] const std::string& message() const { return _message; }

    /**
     * @brief Cross-domain NotFound predicate.
     *
     * If more cross-domain predicates accumulate, replace this if-constexpr
     * chain with a named visitor struct.
     *
     * @return bool
     */
    [[nodiscard]] bool is_not_found() const
    {
        return std::visit(
            [](auto code)
            {
                using T = decltype(code);
                if constexpr (std::is_same_v<T, err::DbError>)
                    return code == err::DbError::NotFound;
                if constexpr (std::is_same_v<T, err::RepoError>)
                    return code == err::RepoError::NotFound;
                if constexpr (std::is_same_v<T, err::StoreError>)
                    return code == err::StoreError::NotFound;
                if constexpr (std::is_same_v<T, err::ServiceError>)
                    return code == err::ServiceError::NotFound;
                if constexpr (std::is_same_v<T, err::InputError>)
                    return code == err::InputError::NotFound;
                return false;
            },
            _kind
        );
    }
};

// ─── std::formatter<Error>
// ────────────────────────────────────────────────────

/// @cond DOXYGEN_IGNORE
/**
 * @brief Custom formatter for Error that produces "[Domain::Code] Message".
 *
 * Relies on err::KindFormatter for the "[Domain::Code]" part. Used by
 * Result::unwrap() when unwrapping an Err.
 */
template <>
struct std::formatter<Error>
{
    static constexpr auto parse(std::format_parse_context& ctx)
    {
        return ctx.begin();
    }

    static auto format(const Error& error, std::format_context& ctx)
    {
        return std::format_to(
            ctx.out(),
            "[{}] {}",
            std::visit(err::KindFormatter{}, error.kind()),
            error.message()
        );
    }
};
/// @endcond

// ─── Convenience alias
// ────────────────────────────────────────────────────────

/**
 * @brief Canonical result type for MolarTracker operations.
 *
 * @tparam T Success value type (use void for operations that succeed
 * silently)
 */
template <typename T>
using AppResult = Result<T, Error>;

#endif   // __UTILS__INCLUDE__UTILS__RESULT__ERROR_HPP__
