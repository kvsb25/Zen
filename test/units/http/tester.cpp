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
    
    // form res with http::Response module
    http::Response* test_res = new http::Response();
    test_res->send(R"({"message":"API Framework Working"})");

    // match res with ref_res, log false for mismatch
        // test cases

        
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