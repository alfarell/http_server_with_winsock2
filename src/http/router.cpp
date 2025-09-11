#include "router.hpp"

HttpRouter::HttpRouter() : routes() {}

HttpRouter::~HttpRouter() {
    routes.clear();
}

void HttpRouter::get(const std::string& path, HandlerFunc handler) {
    this->routes[path][HTTP_GET] = std::move(handler);
}

void HttpRouter::post(const std::string& path, HandlerFunc handler) {
    this->routes[path][HTTP_POST] = std::move(handler);
}

void HttpRouter::put(const std::string& path, HandlerFunc handler) {
    this->routes[path][HTTP_PUT] = std::move(handler);
}

void HttpRouter::patch(const std::string& path, HandlerFunc handler) {
    this->routes[path][HTTP_PATCH] = std::move(handler);
}

void HttpRouter::del(const std::string& path, HandlerFunc handler) {
    this->routes[path][HTTP_DELETE] = std::move(handler);
}

void HttpRouter::callRoute(const HttpRequest& request,
                           const HttpResponse& response) const {
    auto pathIt = this->routes.find(request.path);
    if (pathIt != this->routes.end()) {
        auto methodIt = pathIt->second.find(request.method);
        if (methodIt != pathIt->second.end()) {
            return methodIt->second(request, response);
        }
    }

    response.setStatus(HttpStatus::NotFound);
    response.addHeader("Content-Type: text/plain");
    response.setBody("Page Not Found");
    response.sendResponse();
}