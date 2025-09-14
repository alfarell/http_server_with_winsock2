#include <iostream>
#include <string>

#include "http/router.hpp"
#include "http/server.hpp"

#define SERVER_HOST "127.0.0.1"
#define SERVER_PORT 3021

int main() {
    HttpRouter router{};
    router.get("/", [](const HttpRequest&, const HttpResponse& response) {
        response.setStatus(HttpStatus::OK)
            .addHeader("Content-Type: text/plain")
            .writeBody("Hello from server!")
            .sendResponse();
    });
    router.get("/test", [](const HttpRequest&, const HttpResponse& response) {
        response.setStatus(HttpStatus::OK)
            .addHeader("Content-Type: text/plain")
            .writeBody("/test path")
            .sendResponse();
    });

    HttpServer server = HttpServer();
    server.setRouter(router);
    server.startServer(SERVER_HOST, SERVER_PORT);

    return 0;
}