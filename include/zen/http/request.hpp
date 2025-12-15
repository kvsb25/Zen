#pragma once
#include "utils.hpp"

namespace http{
    struct Request{
    public:
        std::string method;
        std::string path;
        std::unordered_map<std::string, std::vector<std::string>> query;
        std::unordered_map<std::string, std::string> params;
        std::string version;
        std::unordered_map<std::string, std::string> headers;
        std::string body;

        Request(std::string& req_string);
    };
}