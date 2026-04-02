#pragma once
#include "utils.hpp"
#include <vector>

namespace http{
    class ContentNegotiator;
    struct Request;
    
    struct Response{
    private:
        const Request* req;
    public: // public only for test purposes
        int status_code;
        std::string status_message;
        std::unordered_map<std::string, std::string> headers;
        std::string body;
    public:
        void initErrRes(int status_code, const std::string& message);
        Response();
        Response(const Request& req);
        Response& send(const std::string& data);
        Response& json(const std::string& data);
        Response& sendFile(const std::string& filePath);
        Response& status(const int& code);
        Response& header(std::pair<std::string, std::string>& key_value_pair);
        // Response& setHeader(std::string key, std::string value);
        Response& header(std::vector<std::pair<std::string, std::string>>& headers);
        Response& format(const FormatHandlerMap& handler_map);
        void redirect(const std::string& absolute_url);
        std::string construct() const;
    };
}