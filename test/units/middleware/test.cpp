#include "tester.hpp"

int main(){

    std::string req_string =
        "GET / HTTP/1.1\r\n"
        "Host: localhost:3000\r\n"
        "User-Agent: MicroFrameworkTest/1.0\r\n"
        "Accept: application/json\r\n"
        "Connection: keep-alive\r\n"
        "\r\n";
    // http::Request* req = new http::Request(req_string);
    http::Request req(req_string);
    // http::Response* res = new http::Response();
    http::Response res;

    middleware::Middleware* pm = new middleware::PathMiddleware("GET", "/", [](http::Request req, http::Response res){
        std::cout<< "req.method: " << req.method << std::endl 
            << "req.path: " << req.path << std::endl
            << "req.body: " << req.body << std::endl;
            res.json("{\"message\": \"hello\"}");
            return;
    });

    // std::vector<std::unique_ptr<middleware::Middleware>> pipe;

    std::vector<middleware::Middleware*> pipe;
    pipe.push_back(pm);

    for(auto m : pipe){
        if(m->type == middleware::Type::DEFAULT){
            middleware::DefaultMiddleware* dm = static_cast<middleware::DefaultMiddleware*>(m);
            dm->handler(req, res);
        } else if(m->type == middleware::Type::PATH){
            middleware::PathMiddleware* pm = static_cast<middleware::PathMiddleware*>(m);
            if(pm->match(req)){
                pm->handler(req, res);
                break;
            } else {
                std::cout << "Match result: " << pm->match(req);
            }
        }
    }

    return 0;
}