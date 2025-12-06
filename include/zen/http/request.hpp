#pragma once
#include "utils.hpp"

namespace http{
    struct Request{
        std::string method;
        std::string path;
        std::unordered_map<std::string, std::string> query;
        std::unordered_map<std::string, std::string> params;
        std::string version;
        std::unordered_map<std::string, std::string> headers;
        std::string body;
    };
}