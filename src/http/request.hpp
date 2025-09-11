#pragma once

#include <string>

#include "core.hpp"

struct HttpRequest {
    std::string method;
    std::string path;
    std::string query;
    std::string headers;
    std::string body;

    static HttpRequest requestParser(const std::string& request);
};
