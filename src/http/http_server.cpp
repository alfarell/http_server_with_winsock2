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

    HttpRequest httpRequest = HttpRequest::requestParser(request);

    if (httpRequest.method == HTTP_GET) {
        HttpResponse httpResponse;
        httpResponse.setStatus(HttpStatus::OK);
        httpResponse.addHeader("Content-Type: text/plain");
        httpResponse.setBody("Hello from server!");

        this->sendResponse(httpResponse);

        return;
    }

    HttpResponse httpResponse;
    httpResponse.setStatus(HttpStatus::MethodNotAllowed);
    httpResponse.addHeader("Content-Type: text/plain");
    httpResponse.setBody("");

    this->sendResponse(httpResponse);
}

void HttpServer::sendResponse(HttpResponse& response) {
    std::string resp = response.getResponse();

    if (send(this->clientSocket, resp.c_str(), resp.length(), 0) ==
        SOCKET_ERROR) {
        std::cerr << "Send failed: " << WSAGetLastError() << "\n";
        this->cleanup();
    }
}

void HttpServer::cleanup() {
    closesocket(this->listenSocket);
    closesocket(this->clientSocket);
    WSACleanup();
}