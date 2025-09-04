#pragma once

#include <string>

#include "core.hpp"

class HttpResponse {
   private:
    HttpStatus status;
    std::string headers;
    std::string body;

   public:
    HttpResponse();
    ~HttpResponse();

    void setStatus(HttpStatus status);
    void addHeader(const std::string& headers);
    void setBody(const std::string& body);
    std::string getResponse();
};
