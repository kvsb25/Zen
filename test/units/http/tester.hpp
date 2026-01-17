#pragma once
#include "../../../include/zen/http/http.hpp"
#include <sstream>

struct ParsedRequest {
    std::string method;
    std::string path;
    std::string version;
    std::unordered_map<std::string, std::string> headers;
    std::string body;
};

struct ParsedResponse {
    std::string version;
    int status_code;
    std::string status_message;
    std::unordered_map<std::string, std::string> headers;
    std::string body;
};

class Tester{
public:
    ParsedRequest ref_req;
    ParsedResponse ref_res;
    std::string ref_req_str;
    std::string ref_res_str;

public:
    Tester(ParsedRequest& ref_parsed_req, ParsedResponse& ref_parsed_res);
    ~Tester();
    bool wireFormatTest(std::string& req_str, std::string& res_str);
};