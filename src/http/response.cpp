#include "response.hpp"

HttpResponse::HttpResponse() : status(HttpStatus::OK), headers(""), body("") {};

HttpResponse::~HttpResponse() {}

void HttpResponse::setStatus(HttpStatus status) {
    this->status = status;
}

void HttpResponse::addHeader(const std::string& headers) {
    this->headers += headers + "\r\n";
}

void HttpResponse::setBody(const std::string& body) {
    this->body = body;
}

std::string HttpResponse::getResponse() {
    std::string statusText;
    switch (this->status) {
        case HttpStatus::OK:
            statusText = "200 OK";
            break;
        case HttpStatus::Created:
            statusText = "201 Created";
            break;
        case HttpStatus::NoContent:
            statusText = "204 No Content";
            break;
        case HttpStatus::NotFound:
            statusText = "404 Not Found";
            break;
        case HttpStatus::MethodNotAllowed:
            statusText = "405 Method Not Allowed";
            break;
        default:
            statusText = "500 Internal Server Error";
            break;
    }

    std::string response =
        "HTTP/1.1 " + statusText + "\r\n" + this->headers +
        "Content-Length: " + std::to_string(this->body.length()) + "\r\n" +
        "Connection: close\r\n" + "\r\n" + this->body;

    return response;
}
