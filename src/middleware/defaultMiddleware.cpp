#include "../../include/zen/middleware/defaultMiddleware.hpp"

namespace middleware{
    DefaultMiddleware::DefaultMiddleware(std::function<void(http::Request& req, http::Response& res)> callback) : Middleware(Type::DEFAULT), handler(callback){};
}