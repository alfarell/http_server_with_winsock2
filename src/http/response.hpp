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

   public:
    HttpResponse(SOCKET* clientSocket);
    ~HttpResponse();

    void setStatus(HttpStatus status) const;
    void addHeader(const std::string& headers) const;
    void setBody(const std::string& body) const;
    void sendResponse() const;
    std::string getResponse() const;
};
