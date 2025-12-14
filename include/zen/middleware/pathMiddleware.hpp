#pragma once
#include <functional>
#include "../http/http.hpp"
#include "utils.hpp"

namespace middleware{
    class PathMiddleware : public Middleware{
    public:
        std::string method;
        std::string path;
        std::function<void(http::Request& req, http::Response& res)> handler;
        PathMiddleware(std::string m , std::string p, std::function<void(http::Request& req, http::Response& res)> callback);
    };
}