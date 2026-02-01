#include "../../include/zen/http/response.hpp"

namespace http
{
    Response::Response()
    {
        status_code = 200;
        status_message = message_for_status.at(200);

        auto iso = []{ auto t=time(nullptr); char b[32]; strftime(b,32,"%Y-%m-%dT%H:%M:%SZ", gmtime(&t)); return std::string(b); }();
        headers["Date"] = iso;
        headers["Connection"] = "close";
        headers["Content-Length"] = "0";
        headers["Content-Type"] = "text/html; charset=UTF-8";
    }

    Response& Response::send(const std::string& data)
    {
        body = data;
        headers["Content-Length"] = std::to_string(data.length());
        return *this;
    }

    Response& Response::json(const std::string& json_string){
        // can apply triming whitespace logic to reduce payload size
        headers["Content-Type"] = "application/json";
        headers["Content-Length"] = std::to_string(json_string.length());
        return *this;
    }

    Response& Response::sendFile(std::string& filePath){
        std::ifstream file(filePath);
        bool success = false;
        if(file.is_open()){
            std::stringstream buff;
            buff << file.rdbuf();
            body = buff.str();
            file.close();
            success = true;
        } else {
            // throw error here; add error handling here
            body = "<h1>Not found</h1>";
        }

        headers["Content-Length"] = success ? std::to_string(body.length()) : "0";

        return *this;
    }

    Response& Response::status(int& code){
        if(valid_codes.count(code)>0){
            status_code = code;
            status_message = message_for_status.at(code);
        }
        return *this;
    }

    Response& Response::header(std::pair<std::string, std::string>& header){
        headers.insert(header);
        return *this;
    }

    // Response& Response::header(std::unordered_map<std::string, std::string>& v_headers){
    //     headers.insert(v_headers.begin(), v_headers.end());
    //     return *this;
    // }

    // Response& Response::setHeader(std::string key, std::string value){
    //     headers.insert({key, value});
    //     return *this;
    // }

    Response& Response::header(std::vector<std::pair<std::string, std::string>>& headers){
        for(auto header : headers){
            this->headers.insert(header);
        }
        return *this;
    }

    void Response::redirect(std::string& url){
        std::pair<std::string, std::string> location_header = {"Location", url};

        headers.insert(location_header);
        status_code = 307;
        status_message = message_for_status.at(307);
        return;
    }

    std::string Response::construct() const {
        std::stringstream res_ss; // ss is for string stream

        std::string delim = "\r\n";
        std::string block_delim = "\r\n\r\n";

        std::stringstream hdr_block_ss; 

        res_ss << "HTTP/1.1 " << status_code << " " << status_message << " " << delim;
        
        for(const auto& header : this->headers){
            res_ss << header.first << ": " << header.second << delim;
        }

        res_ss << delim;

        res_ss << body;

        return res_ss.str();
    }
}