#include "request.hpp"

HttpRequest HttpRequest::requestParser(const std::string& request) {
    HttpRequest httpRequest;

    if (request.empty()) {
        return httpRequest;
    }

    size_t method_end  = request.find(' ');
    httpRequest.method = request.substr(0, method_end);

    size_t path_end = request.find('?', method_end + 1);
    httpRequest.path =
        request.substr(method_end + 1, path_end - method_end - 1);

    size_t query_end = request.find(' ', path_end + 1);
    httpRequest.path = request.substr(path_end + 1, query_end - path_end - 1);

    size_t headers_end = request.find(HTTP_REQUEST_HEADER_EOF);
    httpRequest.headers =
        request.substr(query_end + 1, headers_end - query_end - 1);

    httpRequest.body = request.substr(headers_end + 4);

    return httpRequest;
}
