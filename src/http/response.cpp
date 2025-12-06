#include "../../include/zen/http/response.hpp"

namespace http
{
    Response::Response()
    {
        status_code = 200;
        status_message = message_for_status.at(200);

        auto iso = []{ auto t=time(nullptr); char b[32]; strftime(b,32,"%Y-%m-%dT%H:%M:%SZ", gmtime(&t)); return std::string(b); }();
        headers["date"] = iso;
    }

    Response& Response::send(std::string &data)
    {
        body << data;
        return *this;
    }

    Response& Response::status(int& code){
        if(valid_codes.count(code)>0){
            status_code = code;
            status_message = message_for_status.at(code);
        }
        return *this;
    }

    Response& Response::header(std::pair<std::string, std::string>& pair){
        headers.insert(pair);
        return *this;
    }

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

}