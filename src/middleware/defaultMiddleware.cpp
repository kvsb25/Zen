#include "../../include/zen/middleware/defaultMiddleware.hpp"

namespace middleware{
    class DefaultMiddleware : public Middleware{
    public:
        std::function<void(http::Request& req, http::Response& res)> handler;
        DefaultMiddleware(std::function<void(http::Request& req, http::Response& res)> callback) : Middleware(Type::DEFAULT), handler(callback){};
    };
}