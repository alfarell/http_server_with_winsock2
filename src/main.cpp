#include <winsock2.h>
#include <ws2tcpip.h>

#include <iostream>
#include <string>

#pragma comment(lib, "ws2_32.lib")

#define SERVER_HOST "127.0.0.1"
#define SERVER_PORT 8080

struct HttpRequest {
    std::string method;
    std::string path;
    std::string headers;
    std::string body;
};

HttpRequest requestParser(std::string& request) {
    HttpRequest httpRequest;

    size_t method_end  = request.find(' ');
    httpRequest.method = request.substr(0, method_end);

    size_t path_end = request.find(' ', method_end + 1);
    httpRequest.path =
        request.substr(method_end + 1, path_end - method_end - 1);

    size_t headers_end = request.find("\r\n\r\n");
    httpRequest.headers =
        request.substr(path_end + 1, headers_end - path_end - 1);

    httpRequest.body = request.substr(headers_end + 4);

    return httpRequest;
}

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed.\n";
        return 1;
    }

    SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listenSocket == INVALID_SOCKET) {
        std::cerr << "Error creating socket: " << WSAGetLastError() << "\n";
        WSACleanup();
        return 1;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family      = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_HOST);
    serverAddr.sin_port        = htons(SERVER_PORT);

    if (bind(listenSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) ==
        SOCKET_ERROR) {
        std::cerr << "Bind failed: " << WSAGetLastError() << "\n";
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }

    if (listen(listenSocket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Listen failed: " << WSAGetLastError() << "\n";
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "Server listening on port 8080...\n";

    while (true) {
        SOCKET clientSocket = accept(listenSocket, NULL, NULL);
        if (clientSocket == INVALID_SOCKET) {
            std::cerr << "Accept failed: " << WSAGetLastError() << "\n";
            closesocket(listenSocket);
            WSACleanup();
            return 1;
        }

        std::cout << "Client connected.\n";

        int res;
        const int bufflen = 1024;
        char buffer[bufflen];
        std::string request;

        do {
            res = recv(clientSocket, buffer, bufflen - 1, 0);
            if (res == 0) {
                std::cerr << "Connection closed\n";
                break;
            }
            if (res < 0) {
                std::cerr << "recv failed: " << WSAGetLastError() << '\n';
                break;
            }

            buffer[res] = '\0';
            request.append(buffer, res);

            if (request.find("\r\n\r\n") != std::string::npos) {
                break;
            }
        } while (res > 0);

        HttpRequest httpRequest = requestParser(request);

        if (httpRequest.method == "GET") {
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

            if (send(clientSocket, response.c_str(), (int)response.length(),
                     0) == SOCKET_ERROR) {
                std::cerr << "Send failed: " << WSAGetLastError() << "\n";
                closesocket(clientSocket);
                WSACleanup();
                return 1;
            }

            continue;
        }

        std::string response =
            "HTTP/1.1 405 Method Not Allowed\r\n"
            "Content-Type: text/plain\r\n"
            "Content-Length: 0\r\n"
            "Connection: close\r\n"
            "\r\n";

        if (send(clientSocket, response.c_str(), (int)response.length(), 0) ==
            SOCKET_ERROR) {
            std::cerr << "Send failed: " << WSAGetLastError() << "\n";
            closesocket(clientSocket);
            WSACleanup();
            return 1;
        }
    }

    closesocket(listenSocket);
    WSACleanup();
    return 0;
}