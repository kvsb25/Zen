#include "../../include/zen/middleware/errorMiddleware.hpp"

namespace middleware{

    ErrorMiddleware::ErrorMiddleware(ErrorHandler h) : Middleware(Type::ERR), handler(h){};
}