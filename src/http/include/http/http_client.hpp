#ifndef __HTTP__INCLUDE__HTTP__HTTP_CLIENT_HPP__
#define __HTTP__INCLUDE__HTTP__HTTP_CLIENT_HPP__

#include <expected>

#include "http_error.hpp"
#include "http_request.hpp"

namespace http
{

    /**
     * @brief Represents an HTTP client
     *
     */
    class HttpClient
    {
       public:
        [[nodiscard]]
        static std::expected<HttpResponse, HttpError> get(
            const HttpRequest& request
        );

       private:
        [[nodiscard]]
        static size_t _writeCallback(
            char*        ptr,
            size_t       size,
            size_t       nMembers,
            std::string* data
        );
    };

}   // namespace http

#endif   // __HTTP__INCLUDE__HTTP__HTTP_CLIENT_HPP__
