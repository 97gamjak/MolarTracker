#include "error/http_error.hpp"

/**
 * @brief Constructs an HttpError object with the specified type, message,
 * status code, and response headers.
 *
 * @param type The type of the HTTP error.
 * @param message A descriptive message for the error.
 * @param statusCode The HTTP status code associated with the error (for
 * BadStatus only).
 * @param responseHeaders A map of response headers from the HTTP request.
 */
HttpError::HttpError(
    HttpErrorType                      type,
    std::string                        message,
    int                                statusCode,
    std::map<std::string, std::string> responseHeaders
)
    : Error<HttpErrorType>(type, message),
      _statusCode{statusCode},
      _responseHeaders{std::move(responseHeaders)}
{
}

/**
 * @brief Constructs an HttpError object with the specified type and message.
 *
 * @param type The type of the HTTP error.
 * @param message A descriptive message for the error.
 */
HttpError::HttpError(HttpErrorType type, std::string message)
    : Error<HttpErrorType>(type, message), _statusCode{0}
{
}

/**
 * @brief Gets the response headers associated with the HTTP error, this
 * function returns a const reference to the map of response headers, allowing
 * for retrieval of the headers for display or logging purposes.
 *
 * @return const std::map<std::string, std::string>& A const reference to the
 * map of response headers associated with the HTTP error.
 */
const std::map<std::string, std::string>& HttpError::getResponseHeaders() const
{
    return _responseHeaders;
}

/**
 * @brief Converts the HttpError to a string representation, including the
 * error type, message, status code, and response headers.
 *
 * @return std::string A string representation of the HttpError object.
 */
std::string HttpError::toString() const
{
    return std::format(
        "HttpError(type={}, message={}, statusCode={}, responseHeaders={})",
        getTypeStr(),
        getMessage(),
        _statusCode,
        [&]()
        {
            std::string headersStr;
            for (const auto& [key, value] : _responseHeaders)
            {
                headersStr += std::format("{}: {}, ", key, value);
            }
            return headersStr;
        }()
    );
}