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
        virtual ~Middleware() = default;
    protected: // protected constructor is used as this class is only an abstract class and we don't want objects to be instantiated from the class
        Middleware(Type t):type(t){};
    };
}