#include "tester.hpp"

Tester::Tester(ParsedRequest &ref_parsed_req, ParsedResponse& ref_parsed_res) : ref_req(ref_parsed_req), ref_res(ref_parsed_res)
{
    //
}

Tester::~Tester()
{
}

void print_headers(std::unordered_map<std::string, std::string>& headers){
    
    std::cout << "key-value pairs: " << std::endl;

    for(auto header: headers){
        std::cout << header.first << ':' << header.second << ';' << "[len:"<< header.first.length() << "," << header.second.length() <<"]" <<std::endl;
    }
    std::cout << "========no. of headers: " << headers.size() << "========" << std::endl;

    std::cout << "ASCII Codes for header values: " << std::endl;
    for(auto header: headers){
        std::cout << header.first << ':' ;
        for(auto c : header.second){
            std::cout << (int)c << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
    std::cout << "================" << std::endl;
    std::cout << std::endl;
}

bool Tester::wireFormatTest(std::string& req_str, std::string& res_str)
{
    bool pass = true;
    // form req with http::Request module
    http::Request* test_req = new http::Request(req_str);
    
    // match req with ref_req, log false for mismatch
        // test cases
        if(test_req->method != ref_req.method){
            pass = false;
            std::cout << "ERROR: Request 'method' mismatch with ref\n\n"
                << "test_req->method:\n" << test_req->method << std::endl
                << "ref_req.method:\n" << ref_req.method << std::endl;
        }
        if(test_req->body != ref_req.body){
            pass = false;
            std::cout << "ERROR: Request 'body' mismatch with ref\n\n"
                << "test:\n " << test_req->body << std::endl
                << "ref:\n " << ref_req.body << std::endl;
        }
        if(test_req->headers != ref_req.headers){
            pass = false;
            std::cout << "ERROR: Request 'headers' mismatch with ref\n\n"
                << "test:\n";
            print_headers(test_req->headers);
            std::cout<<"ref:\n";
            print_headers(ref_req.headers);
            std::cout<<std::endl;
        }
        if(test_req->path != ref_req.path){
            pass = false;
            std::cout << "ERROR: Request 'path' mismatch with ref\n\n"
                << "test:\n" << test_req->path << std::endl
                << "ref:\n" << ref_req.path << std::endl;
        }
    
    // form res with http::Response module
    http::Response* test_res = new http::Response();
    test_res->json(R"({ "message": "API Framework Working" })");
    test_res->construct();
    // match res with ref_res, log false for mismatch
        // test cases
        if(test_res->status_code != ref_res.status_code){
            pass = false;
            std::cout<< "ERROR: Response 'status code' mismatch with ref\n\n"
                << "test_res->status_code:\n" << test_res->status_code << std::endl
                << "ref_res.status_code:\n" << ref_res.status_code << std::endl;
        }
        if(test_res->status_message != ref_res.status_message){
            pass = false;
            std::cout << "ERROR: Response 'status message' mismatch with ref\n\n"
                << "test_res->status_message:\n" << test_res->status_message <<std::endl
                << "ref_res.status_message:\n" << ref_res.status_message << std::endl; 
        }
        if(test_res->headers != ref_res.headers){
            pass = false;
            std::cout << "ERROR: Response 'headers' mismatch with ref\n\n"
                << "test_res->headers: \n";

            print_headers(test_res->headers);
            
            std::cout << "ref_res.headers: \n";

            print_headers(ref_res.headers);
            std::cout<<std::endl;
        }
        
    delete test_req;
    delete test_res;

    return pass;
}

/* Request
GET /api/users?limit=10&page=2 HTTP/1.1\n
Host: localhost:3000\n
User-Agent: MicroFrameworkTest/1.0
Accept: application/json
Connection: keep-alive
 */

/* Response (expected)
HTTP/1.1 200 OK
Content-Type: application/json
Connection: close
Content-Length: 38

{ "message": "API Framework Working" }
*/