#pragma once

#include <string>

#define HTTP_REQUEST_HEADER_EOF "\r\n\r\n"

#define HTTP_GET "GET"
#define HTTP_POST "POST"
#define HTTP_PUT "PUT"
#define HTTP_PATCH "PATCH"
#define HTTP_DELETE "DELETE"

struct HttpRequest {
    std::string method;
    std::string path;
    std::string headers;
    std::string body;

    static HttpRequest requestParser(const std::string& request);
};
