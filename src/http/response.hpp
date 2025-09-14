#pragma once

#include <winsock2.h>

#include <iostream>
#include <string>

#include "core.hpp"

class HttpResponse {
   private:
    mutable HttpStatus status;
    mutable std::string headers;
    mutable std::string body;

    SOCKET* clientSocket;

    std::string responseString() const;

   public:
    HttpResponse(SOCKET* clientSocket);
    ~HttpResponse();

    const HttpResponse& setStatus(HttpStatus status) const;
    const HttpResponse& addHeader(const std::string& headers) const;
    const HttpResponse& writeBody(const std::string& body) const;
    void sendResponse() const;
};
