#include "tester.hpp"

Tester::Tester(ParsedRequest &ref_parsed_req, ParsedResponse& ref_parsed_res) : ref_req(ref_parsed_req), ref_res(ref_parsed_res)
{
    //
}

Tester::~Tester()
{
}

template <typename T>
void validate(bool& pass, const T& actual, const T& expected, const std::string& field) {
    if (actual != expected) {
        pass = false;
        std::cerr << "ERROR: Mismatch in " << field << "\n"
                  << "Actual: " << actual << "\n"
                  << "Expected: " << expected << "\n\n";
    }
}

void print_headers(std::unordered_map<std::string, std::string>& headers){
    
    std::cout << "key-value pairs: " << std::endl;

    for(auto header: headers){
        std::cout << header.first << ':' << header.second << ';' << "[len:"<< header.first.length() << "," << header.second.length() <<"]" <<std::endl;
    }

    std::cout << "========no. of headers: " << headers.size() << "========" << std::endl 
        << "ASCII Codes for header values: " << std::endl;

    for(auto header: headers){
        std::cout << header.first << ':' ;
        for(auto c : header.second){
            std::cout << (int)c << " ";
        }
        std::cout << std::endl;
    }

    std::cout << std::endl << "================" << std::endl << std::endl;
}

bool Tester::wireFormatTest(std::string& req_str, std::string& res_str)
{
    bool pass = true;
    // form req with http::Request module
    http::Request test_req(req_str);
    
    // match req with ref_req, log false for mismatch
    validate(pass, test_req.method, ref_req.method, "Request method");
    validate(pass, test_req.body, ref_req.body, "Request body");
    validate(pass, test_req.path, ref_req.path, "Request path");
        
    if(test_req.headers != ref_req.headers){
        pass = false;
        std::cout << "ERROR: Request 'headers' mismatch with ref\n\n"
            << "test:\n";
        print_headers(test_req.headers);
        std::cout<<"ref:\n";
        print_headers(ref_req.headers);
        std::cout<<std::endl;
    }
    
    // form res with http::Response module
    http::Response test_res;
    test_res.json(ref_res.body);
    test_res.construct();
    
    // match res with ref_res, log false for mismatch
    validate(pass, test_res.status_code, ref_res.status_code, "Response status code");
    validate(pass, test_res.status_message, ref_res.status_message, "Response status message");
    if(test_res.headers != ref_res.headers){
        pass = false;
        std::cout << "ERROR: Response 'headers' mismatch with ref\n\n"
            << "test_res.headers: \n";

        print_headers(test_res.headers);
        
        std::cout << "ref_res.headers: \n";

        print_headers(ref_res.headers);
        std::cout<<std::endl;
    }

    return pass;
}
