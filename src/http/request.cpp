#include "../../include/zen/http/request.hpp"

namespace http
{
    Request::Request(std::string& raw)
    {
    // parse raw request string
        const std::string def_delim = "\r\n";
        const std::string hdr_delim = "\r\n\r\n";
        
        size_t req_len = raw.length();

        // separate header block from body block of request
        auto hdr_end_pos = raw.find(hdr_delim);
        // get the header block of request
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
            if(!hdr_key.empty() && hdr_key.back() == '\r') hdr_key.pop_back();
            std::string hdr_value = line.substr(colon_pos+1);
            if(!hdr_value.empty() && hdr_value.back() == '\r') hdr_value.pop_back();
            this->headers[hdr_key] = hdr_value;
        }

        // get the request body
        size_t body_pos = hdr_end_pos + hdr_delim.size();
        auto it = this->headers.find("Content-Length");
        if(it != this->headers.end()){
            size_t content_len = 0;
            try{
                content_len = std::stoul(it->second); // string to unsigned long
                // std::cout << "REQUEST CONSTRUCTOR - HTTP REQUEST BODY PARSING" << content_len << std::endl;
            } catch (...) {throw std::runtime_error("Invalid Content-Length");}
            if(body_pos + content_len >= req_len){
                this->body = raw.substr(body_pos, content_len);
            }
        } else {
            // for requests with malformed content header having body
            if(body_pos < req_len){
            this->body = raw.substr(body_pos);
            }
        }
    }

    std::unordered_map<std::string, std::vector<std::string>> parseQueryParams(std::string s){ // s == params strings
        std::cout << "in parseQueryParams" << std::endl;
        std::stringstream ss(s);
        std::vector<std::pair<std::string, std::string>> pairs;
        std::string temp;

        // get the key-value pairs
        while(std::getline(ss, temp, '&')){
            size_t pos = temp.find('=');
            pairs.push_back({temp.substr(0,pos), temp.substr(pos+1)});
        }

        ss.clear();
        std::unordered_map<std::string, std::vector<std::string>> queryParams; 

        for(int i = 0 ; i < pairs.size(); i++){
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