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
    private:
    public:
        void initErrRes(int status_code, const std::string& message);
        Response();
        Response& send(const std::string& data);
        Response& json(const std::string& data);
        Response& sendFile(const std::string& filePath);
        Response& status(const int& code);
        Response& header(std::pair<std::string, std::string>& key_value_pair);
        // Response& setHeader(std::string key, std::string value);
        Response& header(std::vector<std::pair<std::string, std::string>>& headers);
        void redirect(const std::string& absolute_url);
        std::string construct() const;
    };
}