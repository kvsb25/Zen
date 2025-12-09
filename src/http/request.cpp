#include "../../include/zen/http/request.hpp"

namespace http
{
    Request::Request(std::string &raw)
    {
        const std::string def_delim = "\r\n";
        const std::string hdr_delim = "\r\n\r\n";
        
        size_t req_len = raw.length();

        auto hdr_end_pos = raw.find(hdr_delim);
        const std::string hdr_block = raw.substr(0, hdr_end_pos);

        std::istringstream hdr_block_ss(hdr_block);
        std::string start_line;

        if(!std::getline(hdr_block_ss, start_line)){
            // empty request, handle error
        };

        // remove the '\r' at the end as getline's default delim_char is '\n' so the '\r' in '\r\n' in the first line remains in the extracted string;
        if(!start_line.empty() && start_line.back() == '\r') start_line.pop_back();

        // get the first line of request

        std::istringstream temp(start_line);
        if(!(temp >> this->method >> this->path >> this->version)){ // after operation>> the same string stream is returned. So, stream becomes falsy if extraction fails, and truthy if extraction succeeds
            // extraction failed probably due to less number of sub strings separated by space in the first line of raw request string
            // handle error
        };

        // get the headers
        std::string line;
        while(std::getline(hdr_block_ss, line)){
            auto colon_pos = line.find(':');
            if(colon_pos == std::string::npos) continue; // ignore malformed header
            std::string hdr_key = line.substr(0,colon_pos);
            std::string hdr_value = line.substr(colon_pos+1);
            this->headers[hdr_key] = hdr_value;
        }

        // get the request body
        size_t body_pos = hdr_end_pos + hdr_delim.size();
        auto it = this->headers.find("Content-Length");
        if(it != this->headers.end()){
            size_t content_len = 0;
            try{
                content_len = std::stoul(it->second); // string to unsigned long
            } catch (...) {throw std::runtime_error("Invalid Content-Length");}
            if(body_pos + content_len > req_len){
                body = raw.substr(body_pos, content_len);
            }
        } else {
            // for requests with malformed content header having body
            if(body_pos < req_len){
            body = raw.substr(body_pos);
            }
        }
    }
}