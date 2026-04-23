#ifndef __HTTP__INCLUDE__HTTP__CURL_HPP__
#define __HTTP__INCLUDE__HTTP__CURL_HPP__

#include <curl/curl.h>

namespace http
{
    /**
     * @brief RAII wrapper for CURL handle
     *
     * This class provides a way to manage the lifetime of a CURL handle
     * by automatically cleaning up the handle when the CurlGuard goes out of
     * scope.
     */
    class CurlGuard
    {
       private:
        /// CURL handle
        CURL* _handle;

       public:
        explicit CurlGuard(CURL* handle);
        ~CurlGuard();

        CurlGuard(const CurlGuard&)            = delete;
        CurlGuard& operator=(const CurlGuard&) = delete;
        CurlGuard(CurlGuard&&)                 = default;
        CurlGuard& operator=(CurlGuard&&)      = default;
    };

    /**
     * @brief RAII wrapper for curl_slist
     *
     * This class provides a way to manage the lifetime of a curl_slist
     * by automatically freeing the list when the HeaderGuard goes out of scope.
     */
    class HeaderGuard
    {
       private:
        /// Pointer to the curl_slist
        curl_slist** _curl_list;

       public:
        explicit HeaderGuard(curl_slist** list);
        ~HeaderGuard();

        HeaderGuard(const HeaderGuard&)            = delete;
        HeaderGuard& operator=(const HeaderGuard&) = delete;
        HeaderGuard(HeaderGuard&&)                 = delete;
        HeaderGuard& operator=(HeaderGuard&&)      = delete;
    };

}   // namespace http

#endif   // __HTTP__INCLUDE__HTTP__CURL_HPP__
