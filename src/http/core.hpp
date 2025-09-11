#pragma once

#include <string>
#include <unordered_map>

#define HTTP_REQUEST_HEADER_EOF "\r\n\r\n"

#define HTTP_GET "GET"
#define HTTP_POST "POST"
#define HTTP_PUT "PUT"
#define HTTP_PATCH "PATCH"
#define HTTP_DELETE "DELETE"

enum class HttpStatus {
    OK               = 200,
    Created          = 201,
    NoContent        = 204,
    NotFound         = 404,
    MethodNotAllowed = 405
};

#define HTTP_STATUS_TEXT_OK "200 OK"
#define HTTP_STATUS_TEXT_CREATED "201 Created"
#define HTTP_STATUS_TEXT_NO_CONTENT "204 No Content"
#define HTTP_STATUS_TEXT_NOT_FOUND "404 Not Found"
#define HTTP_STATUS_TEXT_METHOD_NOT_ALLOWED "405 Method Not Allowed"
#define HTTP_STATUS_TEXT_INTERNAL_SERVER_ERROR "500 Internal Server Error"

std::string httpStatusToString(HttpStatus status);