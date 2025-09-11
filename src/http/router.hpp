#pragma once

#include <functional>
#include <map>
#include <string>

#include "core.hpp"
#include "request.hpp"
#include "response.hpp"

using HandlerFunc =
    std::function<void(const HttpRequest&, const HttpResponse&)>;

typedef std::string HttpRouterPath;
typedef std::string HttpRouterMethod;

class HttpRouter {
   private:
    std::map<HttpRouterPath, std::map<HttpRouterMethod, HandlerFunc>> routes;

   public:
    HttpRouter();
    ~HttpRouter();

    void get(const std::string& path, HandlerFunc handler);
    void post(const std::string& path, HandlerFunc handler);
    void put(const std::string& path, HandlerFunc handler);
    void patch(const std::string& path, HandlerFunc handler);
    void del(const std::string& path, HandlerFunc handler);

    void callRoute(const HttpRequest& request,
                   const HttpResponse& response) const;
};