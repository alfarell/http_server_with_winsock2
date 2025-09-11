#include <iostream>
#include <string>

#include "http/router.hpp"
#include "http/server.hpp"

#define SERVER_HOST "127.0.0.1"
#define SERVER_PORT 3021

int main() {
    HttpRouter router{};
    router.get("/test", [](HttpRequest, HttpResponse response) {
        response.setStatus(HttpStatus::OK);
        response.addHeader("Content-Type: text/plain");
        response.setBody("Hello from server!");
        response.sendResponse();
    });

    HttpServer server = HttpServer();
    server.setRouter(router);
    server.startServer(SERVER_HOST, SERVER_PORT);

    return 0;
}