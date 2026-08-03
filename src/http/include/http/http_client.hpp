#ifndef __HTTP__INCLUDE__HTTP__HTTP_CLIENT_HPP__
#define __HTTP__INCLUDE__HTTP__HTTP_CLIENT_HPP__

#include <atomic>
#include <filesystem>
#include <functional>

#include "error/http_error.hpp"
#include "http_request.hpp"

namespace http
{

    /// Called periodically during downloadToFile() with the number of bytes
    /// received so far and the total size (0 if unknown).
    using DownloadProgressCallback =
        std::function<void(std::int64_t downloaded, std::int64_t total)>;

    /**
     * @brief Represents an HTTP client
     *
     */
    class HttpClient
    {
       public:
        [[nodiscard]]
        static HttpResult<HttpResponse> get(const HttpRequest& request);

        /**
         * @brief Stream a GET response directly to a file on disk.
         *
         * Intended for large downloads (release assets) where buffering the
         * whole body in memory would be wasteful. Follows redirects (GitHub
         * asset URLs redirect to a signed download host) and reports
         * progress via @p progressCallback if provided. If @p cancelFlag is
         * set to true while the transfer is in progress, the download is
         * aborted and a CurlPerform error is returned. On any failure, the
         * partially-written destination file is removed.
         *
         * @param request The HTTP request to send
         * @param destinationPath Where to write the downloaded file
         * @param progressCallback Optional progress reporting callback
         * @param cancelFlag Optional flag to cooperatively cancel the
         * download
         * @return HttpResult<void>
         */
        [[nodiscard]]
        static HttpResult<void> downloadToFile(
            const HttpRequest&              request,
            const std::filesystem::path&    destinationPath,
            const DownloadProgressCallback& progressCallback = nullptr,
            const std::atomic<bool>*        cancelFlag       = nullptr
        );

        [[nodiscard]]
        static std::string urlEncode(const std::string& value);

       private:
        [[nodiscard]]
        static size_t _writeCallback(
            char*        ptr,
            size_t       size,
            size_t       nMembers,
            std::string* data
        );

        [[nodiscard]]
        static size_t headerCallback(
            char*                               ptr,
            size_t                              size,
            size_t                              nmemb,
            std::map<std::string, std::string>* headers
        );
    };

}   // namespace http

#endif   // __HTTP__INCLUDE__HTTP__HTTP_CLIENT_HPP__
