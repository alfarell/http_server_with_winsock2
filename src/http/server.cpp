#include "server.hpp"

HttpServer::HttpServer()
    : wsaData(),
      listenSocket(INVALID_SOCKET),
      clientSocket(INVALID_SOCKET),
      serverAddr(),
      router(nullptr) {}

HttpServer::~HttpServer() {
    this->cleanup();
}

void HttpServer::startServer(const char* host, int port) {
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
    this->isRunning = true;

    while (this->isRunning) {
        this->handleRequest();
    }

    this->cleanup();
}

void HttpServer::handleRequest() {
    this->clientSocket = accept(this->listenSocket, NULL, NULL);
    if (this->clientSocket == INVALID_SOCKET) {
        std::cerr << "Accept failed: " << WSAGetLastError() << "\n";
        this->cleanup();

        return;
    }

    if (this->router == nullptr) {
        std::cerr << "Router is not set.\n";

        HttpResponse response = HttpResponse(&this->clientSocket);
        response.setStatus(HttpStatus::OK);
        response.addHeader("Content-Type: text/plain");
        response.setBody("");
        response.sendResponse();

        return;
    }

    int res;
    char buffer[DEFAULT_BUFF_LEN];
    std::string request;

    do {
        res = recv(this->clientSocket, buffer, DEFAULT_BUFF_LEN - 1, 0);

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

    if (request.empty()) {
        std::cerr << "Request is empty.\n";

        return;
    }

    HttpRequest httpRequest   = HttpRequest::requestParser(request);
    HttpResponse httpResponse = HttpResponse(&this->clientSocket);

    std::cout << "[" << httpRequest.method << "] ";
    std::cout << httpRequest.path << "?" << httpRequest.search << "\n";

    this->router->callRoute(httpRequest, httpResponse);
}

void HttpServer::cleanup() {
    this->isRunning = false;
    closesocket(this->listenSocket);
    closesocket(this->clientSocket);
    WSACleanup();
}

void HttpServer::setRouter(const HttpRouter& router) {
    this->router = &router;
}