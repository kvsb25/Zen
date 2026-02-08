#include "../../include/zen/middleware/pathMiddleware.hpp"

namespace middleware
{
    PathMiddleware::PathMiddleware(std::string m, std::string p, Handler callback) : Middleware(Type::PATH), handler(callback), method(m) {
        // extract path segments with delim as '/'
        try{
        std::stringstream ss(p);
        std::string path_segment;
        std::vector<std::string> segments;

        while(std::getline(ss, path_segment, '/')){
            if(!path_segment.empty()){
                segments.push_back(path_segment);
            }
        }

        //test
            // std::cout << "CONSTRUCTOR: Path segments: " << std::endl;
            // for(auto const& seg: segments){
            //     std::cout<<seg<<", ";
            // }
        //

        ss.clear();
        
        // construct regex for each seg. Static and Param segments
        // std::stringstream regex_temp;
        std::string regex_str = "^";

        if(segments.empty()){
            regex_str += "/$"; // for path: '/' 
        } else {

            for(auto const& seg : segments){
                //test
                    // std::cout << "CONSTRUCTOR: seg: " << seg << std::endl;
                    // std::cout << "CONSTRUCTOR: (seg[0]==':'): " << (seg[0]==':') << std::endl;
                //
                regex_str += '/';
                if(seg[0] == ':'){
                    // Param seg

                    // regex_temp << "([^/]+)";
                    regex_str += "([^/]+)";
                    this->route.params.push_back(seg.substr(1));
                } else {
                    // Static seg 
                    for(auto const& c : seg){
                        // ignore special characters
                        if(std::string(".^$|()[]*+{}\\").find(c) != std::string::npos){
                            continue;
                        }
                        regex_str += c;
                    }
                }
            }

            // remove trailing query params if any;
            size_t pos = regex_str.find('?');
            if(pos != std::string::npos){
                regex_str = regex_str.substr(0,pos);
            }

            regex_str += "/?$";
        }


        //test
            std::cout << "CONSTRUCTOR: regex pattern string formed: " << regex_str << std::endl;
        // full path regex. 
        // std::regex r(regex_temp.str());
        std::regex r(regex_str);

        // initialize path: Route
        this->route.path_regex = r;
        } catch(const std::runtime_error& e){
            throw;
        }
    }

    bool PathMiddleware::match(http::Request& req){
        try{
        // return true if the request url and method matches the middleware attributes method and path:Route
        bool match = false;
        std::smatch m;

        size_t query_pos = req.path.find('?');
        std::string req_eff_path = (query_pos != std::string::npos) ? req.path.substr(0,query_pos) : req.path;

        // test
            // std::cout << "req.method: " << req.method << std::endl << "this->method: " << this->method << std::endl << std::endl;
            // std::cout << "req.path: " << req.path << std::endl << std::endl;
        //

        // if method of req == method of pathMidd && path of req == path of pathMidd
        if(req.method == this->method && std::regex_match(req_eff_path, m, this->route.path_regex)){ // std::regex_match(stringToBeChecked, std::smatch, regexToBeUsed);
            match = true;

            // set req url params
            for(int i = 1 ; i < m.size() ; i++){
                req.params.insert({this->route.params[i-1], m[i]});
            }

            // set req query params
            if(query_pos != std::string::npos){
                std::string query_params_string = req.path.substr(query_pos+1);
                std::cout << "query_params_string" << query_params_string << std::endl;
                req.query = http::parseQueryParams(query_params_string);
            }
            // set req body here, because suppose a request with route, for which a middleware doesn't exist on the server, is sent then time and space will be consumed to parse the body of the req even though it won't be of use as there is not middleware for it
        }

        return match;
        } catch(std::runtime_error){
            throw;
        }
    }
}