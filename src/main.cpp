#include <iostream>
#include <string>

#include "http/server.hpp"

#define SERVER_HOST "127.0.0.1"
#define SERVER_PORT 8080

int main() {
    HttpServer server;
    server.startServer(SERVER_HOST, SERVER_PORT);

    return 0;
}