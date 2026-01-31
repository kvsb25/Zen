#pragma once
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <sstream>
#include <chrono>
#include <fstream>
// #include <variant>

namespace http
{
    // using queryVal = std::variant<
    //     int,
    //     std::vector<int>, 
    //     std::unordered_map<std::string, std::string>
    // >;

    const std::unordered_map<int, std::string> message_for_status = {
        // --- 1xx Informational ---
        {100, "Continue"},
        {101, "Switching Protocols"},
        {102, "Processing"},
        {103, "Early Hints"},

        // --- 2xx Success ---
        {200, "OK"},
        {201, "Created"},
        {202, "Accepted"},
        {203, "Non-Authoritative Information"},
        {204, "No Content"},
        {205, "Reset Content"},
        {206, "Partial Content"},
        {207, "Multi-Status"},
        {208, "Already Reported"},
        {226, "IM Used"},

        // --- 3xx Redirection ---
        {300, "Multiple Choices"},
        {301, "Moved Permanently"},
        {302, "Found"},
        {303, "See Other"},
        {304, "Not Modified"},
        {305, "Use Proxy"},
        {306, "(Unused)"}, // officially reserved, not used
        {307, "Temporary Redirect"},
        {308, "Permanent Redirect"},

        // --- 4xx Client Error ---
        {400, "Bad Request"},
        {401, "Unauthorized"},
        {402, "Payment Required"},
        {403, "Forbidden"},
        {404, "Not Found"},
        {405, "Method Not Allowed"},
        {406, "Not Acceptable"},
        {407, "Proxy Authentication Required"},
        {408, "Request Timeout"},
        {409, "Conflict"},
        {410, "Gone"},
        {411, "Length Required"},
        {412, "Precondition Failed"},
        {413, "Payload Too Large"},
        {414, "URI Too Long"},
        {415, "Unsupported Media Type"},
        {416, "Range Not Satisfiable"},
        {417, "Expectation Failed"},
        {418, "I'm a teapot"},
        {421, "Misdirected Request"},
        {422, "Unprocessable Content"},
        {423, "Locked"},
        {424, "Failed Dependency"},
        {425, "Too Early"},
        {426, "Upgrade Required"},
        {428, "Precondition Required"},
        {429, "Too Many Requests"},
        {431, "Request Header Fields Too Large"},
        {451, "Unavailable For Legal Reasons"},

        // --- 5xx Server Error ---
        {500, "Internal Server Error"},
        {501, "Not Implemented"},
        {502, "Bad Gateway"},
        {503, "Service Unavailable"},
        {504, "Gateway Timeout"},
        {505, "HTTP Version Not Supported"},
        {506, "Variant Also Negotiates"},
        {507, "Insufficient Storage"},
        {508, "Loop Detected"},
        {510, "Not Extended"},
        {511, "Network Authentication Required"}};

    const std::unordered_set<int> valid_codes = {
        // 1xx
        100, 101, 102, 103,
        // 2xx
        200, 201, 202, 203, 204, 205, 206, 207, 208, 226,
        // 3xx
        300, 301, 302, 303, 304, 305, 307, 308,
        // 4xx
        400, 401, 402, 403, 404, 405, 406, 407, 408, 409,
        410, 411, 412, 413, 414, 415, 416, 417, 418,
        421, 422, 423, 424, 425, 426, 428, 429, 431, 451,
        // 5xx
        500, 501, 502, 503, 504, 505, 506, 507, 508, 510, 511};
    
    template <typename T>
    // std::unordered_map<std::string, queryVal> parseQueryParams(std::string param_string){
    //     // parse query params

    // }

    std::unordered_map<std::string, std::vector<std::string>> parseQueryParams(std::string s){ // s == params strings
        std::stringstream ss(s);
        std::vector<std::pair<std::string, std::string>> pairs;
        std::string temp;

        while(std::getline(ss, temp, '&')){
            size_t pos = temp.find('=');
            pairs.push_back({temp.substr(0,pos), temp.substr(pos+1)});
        }

        ss.clear();
        std::unordered_map<std::string, std::vector<std::string>> queryParams; 

        for(int i = 0 ; i < s.size(); i++){
            auto [key, value] = pairs[i];
            
            std::regex pat(R"(.\[\])");
            if(std::regex_search(key, pat)){
                size_t pos = key.find('[');
                // if(pos != std::string::npos)
                key = key.substr(0, pos+1);
            }

            std::vector<std::string> values;
            if(value.find(',') != std::string::npos){
                ss << value;

                while(std::getline(ss, value, ',')){
                    values.push_back(value);
                }
            }

            queryParams.insert({key, values});
        }

        return queryParams;
    }
}