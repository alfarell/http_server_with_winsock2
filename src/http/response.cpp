#include "response.hpp"

HttpResponse::HttpResponse(SOCKET* clientSocket)
    : status(HttpStatus::OK),
      headers(""),
      body(""),
      clientSocket(clientSocket) {};

HttpResponse::~HttpResponse() {}

const HttpResponse& HttpResponse::setStatus(HttpStatus status) const {
    this->status = status;

    return *this;
}

const HttpResponse& HttpResponse::addHeader(const std::string& headers) const {
    this->headers += headers + "\r\n";

    return *this;
}

const HttpResponse& HttpResponse::writeBody(const std::string& body) const {
    this->body = body;

    return *this;
}

std::string HttpResponse::responseString() const {
    std::string statusText = httpStatusToString(this->status);
    std::string response =
        "HTTP/1.1 " + statusText + "\r\n" + this->headers +
        "Content-Length: " + std::to_string(this->body.length()) + "\r\n" +
        "Connection: close\r\n" + "\r\n" + this->body;

    return response;
}

void HttpResponse::sendResponse() const {
    std::string resp = this->responseString();

    if (send(*this->clientSocket, resp.c_str(), resp.length(), 0) ==
        SOCKET_ERROR) {
        std::cerr << "Send failed: " << WSAGetLastError() << "\n";
    }
}
