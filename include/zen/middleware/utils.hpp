#pragma once

#include <iostream>

namespace middleware{
    
    using Handler = std::function<void(http::Request& req, http::Response& res)>;
    
    enum class Type{
        DEFAULT,
        PATH
    };

    class Middleware {
    public:
        Type type;
    protected:
        Middleware(Type t):type(t){};
    };
}