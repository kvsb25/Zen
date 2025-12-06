#pragma once
#include "utils.hpp"
#include <vector>

namespace http{
    struct Response{
        int status_code = 200;
        std::string status_message = "OK";
        std::unordered_map<std::string, std::string> headers;
        std::string body;

        Response& send(std::string& data);
        Response& status(int status);
        Response& header(std::pair<std::string, std::string>& key_value_pair);
        Response& header(std::vector<std::pair<std::string, std::string>>& headers);
        Response& redirect(std::string& absolute_url);
    };
}