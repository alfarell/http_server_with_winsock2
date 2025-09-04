#pragma once

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