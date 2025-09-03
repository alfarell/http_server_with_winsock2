#include "http_server.hpp"

HttpServer::HttpServer()
    : wsaData(),
      listenSocket(INVALID_SOCKET),
      clientSocket(INVALID_SOCKET),
      serverAddr() {}

HttpServer::~HttpServer() {
    this->cleanup();
}

void HttpServer::startServer(const char* host, int port) {
    this->wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &this->wsaData) != 0) {
        std::cerr << "WSAStartup failed.\n";

        return;
    }

    this->listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (this->listenSocket == INVALID_SOCKET) {
        std::cerr << "Error creating socket: " << WSAGetLastError() << "\n";
        this->cleanup();

        return;
    }

    this->serverAddr.sin_family      = AF_INET;
    this->serverAddr.sin_addr.s_addr = inet_addr(host);
    this->serverAddr.sin_port        = htons(port);

    if (bind(this->listenSocket, (SOCKADDR*)&this->serverAddr,
             sizeof(this->serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Bind failed: " << WSAGetLastError() << "\n";
        this->cleanup();

        return;
    }

    if (listen(this->listenSocket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Listen failed: " << WSAGetLastError() << "\n";
        this->cleanup();

        return;
    }

    std::cout << "Server listening on port " << port << "...\n";

    while (true) {
        this->handleRequest();
    }
}

void HttpServer::handleRequest() {
    this->clientSocket = accept(this->listenSocket, NULL, NULL);
    if (this->clientSocket == INVALID_SOCKET) {
        std::cerr << "Accept failed: " << WSAGetLastError() << "\n";
        this->cleanup();

        return;
    }

    int res;
    const int bufflen = DEFAULT_BUFF_LEN;
    char buffer[bufflen];
    std::string request;

    do {
        res = recv(this->clientSocket, buffer, bufflen - 1, 0);

        if (res == 0) {
            std::cout << "Connection closed\n";
            break;
        }

        if (res < 0) {
            std::cerr << "Reading request buffer failed: " << WSAGetLastError()
                      << '\n';
            break;
        }

        buffer[res] = '\0';
        request.append(buffer, res);

        if (request.find(HTTP_REQUEST_HEADER_EOF) != std::string::npos) {
            break;
        }
    } while (res > 0);

    HttpRequest httpRequest = HttpRequest::requestParser(request);

    if (httpRequest.method == HTTP_GET) {
        const char* body = "Hello from server!";
        std::string response =
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/plain\r\n"
            "Content-Length: " +
            std::to_string(strlen(body)) +
            "\r\n"
            "Connection: close\r\n"
            "\r\n" +
            std::string(body);

        if (send(this->clientSocket, response.c_str(), (int)response.length(),
                 0) == SOCKET_ERROR) {
            std::cerr << "Send failed: " << WSAGetLastError() << "\n";
            this->cleanup();

            return;
        }

        return;
    }

    std::string response =
        "HTTP/1.1 405 Method Not Allowed\r\n"
        "Content-Type: text/plain\r\n"
        "Content-Length: 0\r\n"
        "Connection: close\r\n"
        "\r\n";

    if (send(this->clientSocket, response.c_str(), (int)response.length(), 0) ==
        SOCKET_ERROR) {
        std::cerr << "Send failed: " << WSAGetLastError() << "\n";
        this->cleanup();

        return;
    }
}

void HttpServer::cleanup() {
    closesocket(this->listenSocket);
    closesocket(this->clientSocket);
    WSACleanup();
}