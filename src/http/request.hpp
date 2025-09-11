#pragma once

#include <iostream>
#include <map>
#include <sstream>
#include <string>

#include "core.hpp"

struct HttpRequest {
    std::string method;
    std::string path;
    std::string search;
    std::map<std::string, std::string> queries;
    std::string version;
    std::map<std::string, std::string> headers;
    std::string body;

    static HttpRequest requestParser(const std::string& request);
};
