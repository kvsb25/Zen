#pragma once
#include "utils.hpp"
#include "response.hpp"
#include "request.hpp"

namespace http{
    class Parser{
        static Request* parse(const std::string& req);
        static std::string construct(const Response* &res);
    };
}