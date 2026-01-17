#include "tester.hpp"

Tester::Tester(ParsedRequest &ref_parsed_req, ParsedResponse& ref_parsed_res) : ref_req(ref_parsed_req), ref_res(ref_parsed_res)
{
    //
}

Tester::~Tester()
{
}

bool Tester::wireFormatTest(std::string& req_str, std::string& res_str)
{
    bool pass = true;
    // form req with http::Request module
    http::Request* test_req = new http::Request(req_str);
    
    // match req with ref_req, log false for mismatch
        // test cases
        if(test_req->method != ref_req.method){pass = false; std::cout << "ERROR: Request method mismatch with ref";}
        if(test_req->body != ref_req.body){pass = false; std::cout << "ERROR: Request body mismatch with ref";}
        if(test_req->headers != ref_req.headers){pass = false; std::cout << "ERROR: Request headers mismatch with ref";}
        if(test_req->path != ref_req.path){pass = false; std::cout << "ERROR: Request path mismatch with ref";}
    
    // form res with http::Response module
    http::Response* test_res = new http::Response();
    test_res->json(R"({ "message": "API Framework Working" })");
    test_res->construct();
    // match res with ref_res, log false for mismatch
        // test cases
        if(test_res->status_code != ref_res.status_code){pass = false; std::cout<< "ERROR: Response status code mismatch with ref";}
        if(test_res->status_message != ref_res.status_message){pass = false; std::cout << "ERROR: Response status message mismatch with ref";}
        if(test_res->headers != ref_res.headers){pass = false; std::cout << "ERROR: Response headers mismatch with ref";}
        
    delete test_req;
    delete test_res;
}

/* Request
GET /api/users?limit=10&page=2 HTTP/1.1
Host: localhost:3000
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