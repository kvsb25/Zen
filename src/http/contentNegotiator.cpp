#include "../../include/zen/http/contentNegotiator.hpp"

namespace http{
    FormatPriority ContentNegotiator::parse(const std::string& head_val){
        FormatPriority pq;
        std::stringstream ss(head_val);
        std::string temp;
        while(std::getline(ss, temp, ',')){
            // // TRIM SPACES
            // std::regex trim_re(R"(^\s+|\s+$)");
            // temp = std::regex_replace(temp, trim_re, "");

            float priority = 1;
            size_t semi_pos = temp.find(';');
            if(semi_pos != std::string::npos){
                if((std::string(temp, semi_pos+1)).length() < 3){
                    // the request header is malformed
                    // handle error
                }
                std::string s(temp, semi_pos+1);
                std::string pri_val_str(s, s.find('=')+1);
                priority = std::stof(pri_val_str);
            }
            std::string content_type(temp, 0, semi_pos+1);
            pq.push({priority, content_type});
        }

        return pq;
    }

    std::string ContentNegotiator::match(const FormatHandlerMap& mp, const FormatPriority& vals){
        std::string result = "DEFAULT";

        for(int i = 0; i < vals.size(); i++){
            auto it = mp.find(vals.top().second);
            if(it != mp.end()){
                result = (*it).first;
            }
        }

        return result;
    }
}