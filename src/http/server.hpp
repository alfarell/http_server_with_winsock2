#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>

#include <iostream>
#include <string>

#include "request.hpp"
#include "response.hpp"
#include "router.hpp"

#define DEFAULT_BUFF_LEN 1024

class HttpServer {
   private:
    WSADATA wsaData;
    SOCKET listenSocket;
    SOCKET clientSocket;
    sockaddr_in serverAddr;
    bool isRunning;

    const HttpRouter* router;

   private:
    void handleRequest();
    void cleanup();

   public:
    HttpServer();
    ~HttpServer();

    void startServer(const char* host, int port);
    void setRouter(const HttpRouter& router);
};
