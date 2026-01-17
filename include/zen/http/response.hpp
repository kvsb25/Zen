#pragma once
#include "utils.hpp"
#include <vector>

namespace http{
    struct Response{
    public: // public only for test purposes
        int status_code;
        std::string status_message;
        std::unordered_map<std::string, std::string> headers;
        std::string body;
    public:
        Response();
        Response& send(const std::string& data);
        Response& json(const std::string& data);
        Response& sendFile(std::string& filePath);
        Response& status(int& code);
        Response& header(std::pair<std::string, std::string>& key_value_pair);
        // Response& setHeader(std::string key, std::string value);
        Response& header(std::vector<std::pair<std::string, std::string>>& headers);
        void redirect(std::string& absolute_url);
        std::string construct() const;
    };
}