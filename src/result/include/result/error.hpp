#ifndef __RESULT__INCLUDE__RESULT__ERROR_HPP__
#define __RESULT__INCLUDE__RESULT__ERROR_HPP__

#include <cstdint>
#include <format>
#include <string>
#include <type_traits>
#include <variant>

#include "result/result.hpp"

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
// -Werror=switch-enum will flag any visitor that does not handle the new arm.

namespace err
{

    enum class Db : std::uint8_t
    {
        NotFound,
        ConstraintViolation,
        QueryFailed,
        ConnectionFailed,
    };

    enum class Repo : std::uint8_t
    {
        NotFound,
        InvalidState,
        DuplicateKey,
    };

    enum class Store : std::uint8_t
    {
        NotFound,
        Conflict,
        InvalidState,
    };

    enum class Service : std::uint8_t
    {
        NotFound,
        InvalidState,
        Unauthorized,
    };

    enum class Input : std::uint8_t
    {
        NotFound,
        MissingField,
        InvalidValue,
        OutOfRange,
    };

    using Kind = std::variant<Db, Repo, Store, Service, Input>;

    // ── domain_name ──────────────────────────────────────────────────────────

    [[nodiscard]] constexpr std::string_view domain_name(Db) noexcept
    {
        return "Db";
    }

    [[nodiscard]] constexpr std::string_view domain_name(Repo) noexcept
    {
        return "Repo";
    }

    [[nodiscard]] constexpr std::string_view domain_name(Store) noexcept
    {
        return "Store";
    }

    [[nodiscard]] constexpr std::string_view domain_name(Service) noexcept
    {
        return "Service";
    }

    [[nodiscard]] constexpr std::string_view domain_name(Input) noexcept
    {
        return "Input";
    }

    // ── code_name ────────────────────────────────────────────────────────────

    [[nodiscard]] constexpr std::string_view code_name(Db c) noexcept
    {
        switch (c)
        {
            case Db::NotFound:
                return "NotFound";
            case Db::ConstraintViolation:
                return "ConstraintViolation";
            case Db::QueryFailed:
                return "QueryFailed";
            case Db::ConnectionFailed:
                return "ConnectionFailed";
        }
        std::unreachable();
    }

    [[nodiscard]] constexpr std::string_view code_name(Repo c) noexcept
    {
        switch (c)
        {
            case Repo::NotFound:
                return "NotFound";
            case Repo::InvalidState:
                return "InvalidState";
            case Repo::DuplicateKey:
                return "DuplicateKey";
        }
        std::unreachable();
    }

    [[nodiscard]] constexpr std::string_view code_name(Store c) noexcept
    {
        switch (c)
        {
            case Store::NotFound:
                return "NotFound";
            case Store::Conflict:
                return "Conflict";
            case Store::InvalidState:
                return "InvalidState";
        }
        std::unreachable();
    }

    [[nodiscard]] constexpr std::string_view code_name(Service c) noexcept
    {
        switch (c)
        {
            case Service::NotFound:
                return "NotFound";
            case Service::InvalidState:
                return "InvalidState";
            case Service::Unauthorized:
                return "Unauthorized";
        }
        std::unreachable();
    }

    [[nodiscard]] constexpr std::string_view code_name(Input c) noexcept
    {
        switch (c)
        {
            case Input::NotFound:
                return "NotFound";
            case Input::MissingField:
                return "MissingField";
            case Input::InvalidValue:
                return "InvalidValue";
            case Input::OutOfRange:
                return "OutOfRange";
        }
        std::unreachable();
    }

    // ── KindFormatter ────────────────────────────────────────────────────────

    /**
     * @brief Named visitor struct that formats an err::Kind as "Domain::Code".
     *
     * Follows the project's named-visitor-struct convention; avoids overloaded
     * lambdas. Use with std::visit:
     *
     *   std::string s = std::visit(err::KindFormatter{}, kind);
     */
    struct KindFormatter
    {
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
 * Flows through all application layers via Result<T, Error> / AppResult<T>.
 * The err::Kind variant enforces exhaustive handling at every visit site
 * (-Werror=switch-enum flags missing arms).
 *
 * Static factories make construction at each layer self-documenting:
 *   return Err{Error::repo(err::Repo::NotFound, "Profile not found")};
 */
struct Error
{
    /// Domain and code of the failure
    err::Kind _kind;
    /// Human-readable description of the failure
    std::string _message;

    [[nodiscard]] static Error db(err::Db code, std::string msg)
    {
        return {code, std::move(msg)};
    }

    [[nodiscard]] static Error repo(err::Repo code, std::string msg)
    {
        return {code, std::move(msg)};
    }

    [[nodiscard]] static Error store(err::Store code, std::string msg)
    {
        return {code, std::move(msg)};
    }

    [[nodiscard]] static Error service(err::Service code, std::string msg)
    {
        return {code, std::move(msg)};
    }

    [[nodiscard]] static Error input(err::Input code, std::string msg)
    {
        return {code, std::move(msg)};
    }

    [[nodiscard]] const err::Kind&   kind() const noexcept { return _kind; }
    [[nodiscard]] const std::string& message() const noexcept
    {
        return _message;
    }

    /**
     * @brief Cross-domain NotFound predicate.
     *
     * If more cross-domain predicates accumulate, replace this if-constexpr
     * chain with a named visitor struct.
     */
    [[nodiscard]] bool is_not_found() const noexcept
    {
        return std::visit(
            [](auto code) noexcept
            {
                using T = decltype(code);
                if constexpr (std::is_same_v<T, err::Db>)
                    return code == err::Db::NotFound;
                if constexpr (std::is_same_v<T, err::Repo>)
                    return code == err::Repo::NotFound;
                if constexpr (std::is_same_v<T, err::Store>)
                    return code == err::Store::NotFound;
                if constexpr (std::is_same_v<T, err::Service>)
                    return code == err::Service::NotFound;
                if constexpr (std::is_same_v<T, err::Input>)
                    return code == err::Input::NotFound;
                return false;
            },
            _kind
        );
    }
};

// ─── std::formatter<Error>
// ────────────────────────────────────────────────────

template <>
struct std::formatter<Error>
{
    constexpr auto parse(std::format_parse_context& ctx) { return ctx.begin(); }

    auto format(const Error& e, std::format_context& ctx) const
    {
        return std::format_to(
            ctx.out(),
            "[{}] {}",
            std::visit(err::KindFormatter{}, e.kind()),
            e.message()
        );
    }
};

// ─── Convenience alias
// ────────────────────────────────────────────────────────

/**
 * @brief Canonical result type for MolarTracker operations.
 *
 * @tparam T Success value type (use void for operations that succeed silently)
 */
template <typename T>
using AppResult = Result<T, Error>;

#endif   // __RESULT__INCLUDE__RESULT__ERROR_HPP__
