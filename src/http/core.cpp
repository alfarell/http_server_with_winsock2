#include "core.hpp"

std::string httpStatusToString(HttpStatus status) {
    switch (status) {
        case HttpStatus::OK:
            return HTTP_STATUS_TEXT_OK;
        case HttpStatus::Created:
            return HTTP_STATUS_TEXT_CREATED;
        case HttpStatus::NoContent:
            return HTTP_STATUS_TEXT_NO_CONTENT;
        case HttpStatus::NotFound:
            return HTTP_STATUS_TEXT_NOT_FOUND;
        case HttpStatus::MethodNotAllowed:
            return HTTP_STATUS_TEXT_METHOD_NOT_ALLOWED;
        default:
            return HTTP_STATUS_TEXT_INTERNAL_SERVER_ERROR;
    }
}