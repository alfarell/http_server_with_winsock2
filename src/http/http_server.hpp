#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>

#include <iostream>
#include <string>

#include "http_request.hpp"

#pragma comment(lib, "ws2_32.lib")

#define DEFAULT_BUFF_LEN 1024

class HttpServer {
   private:
    WSADATA wsaData;
    SOCKET listenSocket;
    SOCKET clientSocket;
    sockaddr_in serverAddr;

   private:
    void handleRequest();
    void cleanup();

   public:
    HttpServer();
    ~HttpServer();

    void startServer(const char* host, int port);
};
