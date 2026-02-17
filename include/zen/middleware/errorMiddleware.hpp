#pragma once
#include "utils.hpp"
#include "err.hpp"

namespace middleware{
    using ErrorHandler = std::function<void(const ZenErr& e, http::Request& req, http::Response& res)>;

    class ErrorMiddleware : public Middleware{
    public:
        ErrorHandler handler;
        ErrorMiddleware(ErrorHandler h);
    };
}