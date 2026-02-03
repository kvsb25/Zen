#include "tester.hpp"

int main(){

    std::string req_string =
        "GET /vendor/product/1234 HTTP/1.1\r\n"
        "Host: localhost:3000\r\n"
        "User-Agent: MicroFrameworkTest/1.0\r\n"
        "Accept: application/json\r\n"
        "Content-Type: application/json\r\n"
        "Content-Length: 33\r\n"
        "Connection: keep-alive\r\n"
        "\r\n"
        "{\"message\":\"vendor product name\"}";
    // http::Request* req = new http::Request(req_string);
    http::Request req(req_string);
    // http::Response* res = new http::Response();
    http::Response res;

    // make sure the request and response objects sent to the handler are references
    middleware::Middleware* pm = new middleware::PathMiddleware("GET", "/", [](http::Request& req, http::Response& res){
        std::cout<< "req.method: " << req.method << std::endl 
            << "req.path: " << req.path << std::endl
            << "req.body: " << req.body << std::endl;
        res.json("{\"message\": \"hello\"}");
        return;
    });

    auto handler = [](http::Request& req, http::Response& res){
        std::cout<< "req.method: " << req.method << std::endl 
            << "req.path: " << req.path << std::endl
            << "req.body: " << req.body << std::endl;
        if(!req.query.empty()) {
            for(auto const& p : req.query){
                std::cout << "key: " << p.first  << std::endl;
                std::cout << "value: " << !p.second.empty() << std::endl;
                if(!p.second.empty()){
                    for(auto const& v : p.second){
                        std::cout << v << std::endl;
                    }
                }
            }
        }
        std::cout << "req.params" << std::endl;
        for(auto a : req.params){
            std::cout << "key: " << a.first << " value: " << a.second << std::endl;
        }
        std::cout << "Product ID: " << req.params["id"];
        res.json("{\"message\": \"received successfully\"}");
        return;
    };

    middleware::Middleware* pm2 = new middleware::PathMiddleware("GET", "/vendor/product/:id", handler); //?id=123

    middleware::Middleware* dm = new middleware::DefaultMiddleware([](http::Request& req, http::Response& res){
        std::cout << "Default middleware 1" << std::endl;
        return;
    });

    // std::vector<std::unique_ptr<middleware::Middleware>> pipe;

    std::vector<middleware::Middleware*> pipe;
    pipe.push_back(dm);
    pipe.push_back(dm);
    pipe.push_back(pm);
    pipe.push_back(pm2);
    pipe.push_back(dm);

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
                std::cout << "Match result: " << pm->match(req) << std::endl;
            }
        }
    }

    std::cout << std::endl << "Final response string formed: " << std::endl << res.construct() << std::endl;

    return 0;
}