#pragma once
#include "utils.hpp"
#include <vector>
#include <sstream>
#include <chrono>
#include <fstream>

namespace http{
    struct Response{
        int status_code;
        std::string status_message;
        std::unordered_map<std::string, std::string> headers;
        std::string body;

        Response();
        Response& send(std::string& data);
        Response& sendFile(std::string& filePath);
        Response& status(int& code);
        Response& header(std::pair<std::string, std::string>& key_value_pair);
        Response& header(std::vector<std::pair<std::string, std::string>>& headers);
        void redirect(std::string& absolute_url);
    };
}