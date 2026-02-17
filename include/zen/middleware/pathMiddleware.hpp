#pragma once
#include <functional>
#include <regex>
#include "utils.hpp"

namespace middleware{

    struct Route{
        // path's regex
        std::regex path_regex;
        // url params
        std::vector<std::string> params;
    };

    class PathMiddleware : public Middleware{
    public:
        std::string method;
        Route route;
        Handler handler;
    public:
        PathMiddleware(std::string m , std::string p, Handler callback);
        bool match(http::Request& req);
    };
}