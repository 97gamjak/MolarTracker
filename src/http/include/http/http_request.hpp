#ifndef __HTTP__INCLUDE__HTTP__HTTP_REQUEST_HPP__
#define __HTTP__INCLUDE__HTTP__HTTP_REQUEST_HPP__

#include <cstdint>
#include <map>
#include <string>

namespace http
{

    /**
     * @brief Represents an HTTP request
     *
     */
    struct HttpRequest
    {
        /// The timeout for the request in seconds
        static constexpr std::int64_t timeoutSeconds{30};

        /// The URL for the request
        std::string url;

        /// The headers for the request
        std::map<std::string, std::string> headers;
    };

    /**
     * @brief Represents an HTTP response
     *
     */
    struct HttpResponse
    {
        /// The status code for the response
        std::int64_t statusCode = 0;

        /// The body of the response
        std::string body;

        /// The headers for the response
        std::map<std::string, std::string> headers;
    };

}   // namespace http

#endif   // __HTTP__INCLUDE__HTTP__HTTP_REQUEST_HPP__