#include "http/curl.hpp"

namespace http
{

    /**
     * @brief Construct a new Curl Guard:: Curl Guard object
     *
     * @param handle
     */
    CurlGuard::CurlGuard(CURL* handle) : _handle(handle) {}

    /**
     * @brief Destroy the Curl Guard:: Curl Guard object
     *
     */
    CurlGuard::~CurlGuard() { curl_easy_cleanup(_handle); }

    /**
     * @brief Construct a new Header Guard:: Header Guard object
     *
     * @param list
     */
    HeaderGuard::HeaderGuard(curl_slist** list) : _curl_list(list) {}

    /**
     * @brief Destroy the Header Guard:: Header Guard object
     *
     */
    HeaderGuard::~HeaderGuard() { curl_slist_free_all(*_curl_list); }
}   // namespace http
