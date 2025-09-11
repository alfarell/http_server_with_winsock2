#include "request.hpp"

static inline std::string trim(const std::string& s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    size_t end   = s.find_last_not_of(" \t\r\n");
    if (start == std::string::npos || end == std::string::npos) return "";
    return s.substr(start, end - start + 1);
}

std::map<std::string, std::string> parseQuery(const std::string& queryString) {
    std::map<std::string, std::string> queryParams;
    std::stringstream ss(queryString);
    std::string pair;

    while (std::getline(ss, pair, '&')) {
        auto pos = pair.find('=');
        if (pos != std::string::npos) {
            std::string key   = pair.substr(0, pos);
            std::string value = pair.substr(pos + 1);
            queryParams[key]  = value;
        } else {
            queryParams[pair] = "";
        }
    }
    return queryParams;
}

HttpRequest HttpRequest::requestParser(const std::string& request) {
    HttpRequest httpRequest;

    std::istringstream stream(request);
    std::string line;

    // --- Request line ---
    if (!std::getline(stream, line)) return httpRequest;
    line = trim(line);

    std::istringstream urlLine(line);
    urlLine >> httpRequest.method;

    // --- Path ---
    std::string url;
    urlLine >> url;
    urlLine >> httpRequest.version;

    // --- Query Param ---
    std::string::size_type qpos = url.find('?');
    if (qpos != std::string::npos) {
        httpRequest.path    = url.substr(0, qpos);
        httpRequest.search  = url.substr(qpos + 1);
        httpRequest.queries = parseQuery(httpRequest.search);
    } else {
        httpRequest.path = url;
    }

    // --- Headers ---
    while (std::getline(stream, line)) {
        line = trim(line);
        if (line.empty()) break;

        auto pos = line.find(':');
        if (pos != std::string::npos) {
            std::string key          = trim(line.substr(0, pos));
            std::string value        = trim(line.substr(pos + 1));
            httpRequest.headers[key] = value;
        }
    }

    // --- Body ---
    std::string body;
    while (std::getline(stream, line)) {
        body += line + "\n";
    }
    if (!body.empty() && body.back() == '\n') {
        body.pop_back();
    }
    httpRequest.body = body;

    return httpRequest;
}
