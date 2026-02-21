#include "tester.hpp"

void testHandle(http::Request& req, http::Response& res, std::vector<middleware::Middleware*> pipe, int index = 0, const ZenErr& err = ZenErr("", false)){
    
    if(index >= pipe.size()) return;

    middleware::Middleware* mw = pipe[index]; // create a borrowed raw pointer of the unique_ptr at 'index' in pipe // DON'T DELETE mw MANUALLY
    try
    {
        if (!err.isRelevant())
        {
            std::cout << "Error is not relevant!" << std::endl;
            if (mw->type == middleware::Type::DEFAULT)
            {
                auto *dmw = static_cast<middleware::DefaultMiddleware *>(mw); // down cast
                dmw->handler(req, res);
                // give flag -02 or -03 with gcc to leverage tail call optimization
            }
            else if (mw->type == middleware::Type::PATH)
            {
                auto *pmw = static_cast<middleware::PathMiddleware *>(mw); // down cast
                if (pmw->match(req))
                {
                    pmw->handler(req, res);
                    return;
                }
            }
        }
        else
        {
            if (mw->type == middleware::Type::ERR)
            {
                auto *emw = static_cast<middleware::ErrorMiddleware *>(mw);
                emw->handler(err, req, res);
            }
        }
        return testHandle(req, res, pipe, index + 1, err);
    }
    catch (const ZenErr &e)
    {
        if(e.isRelevant())
            return testHandle(req, res, pipe, index + 1, e);
    }
    catch (const std::runtime_error& e){
        throw;
    }
    // delete all pointers on error 

    return;
}

int main(){

    std::string req_string =
        "GET / HTTP/1.1\r\n"
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

    middleware::Middleware* emw = new middleware::ErrorMiddleware([](const ZenErr& e, http::Request& req, http::Response& res){
        // std::cout << e.what() << std::endl;
        res.initErrRes(404, "testing");
        // std::cout << e.getTrace() << std::endl;
        std::cout << " in error middleware: TEST: constructed response: " << res.construct() << std::endl;
        return;
    });

    // make sure the request and response objects sent to the handler are references
    middleware::Middleware* pm = new middleware::PathMiddleware("GET", "/", [](http::Request& req, http::Response& res){
        if(true){
            throw ZenErr("Test error");
        }
        
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
    pipe.push_back(emw);

    testHandle(req, res, pipe);

    // for(auto m : pipe){
    //     if(m->type == middleware::Type::DEFAULT){
    //         middleware::DefaultMiddleware* dm = static_cast<middleware::DefaultMiddleware*>(m);
    //         dm->handler(req, res);
    //     } else if(m->type == middleware::Type::PATH){
    //         middleware::PathMiddleware* pm = static_cast<middleware::PathMiddleware*>(m);
    //         if(pm->match(req)){
    //             pm->handler(req, res);
    //             break;
    //         } else {
    //             std::cout << "Match result: " << pm->match(req) << std::endl;
    //         }
    //     }
    // }

    std::cout << std::endl << "After tesHandle: Final response string formed: " << std::endl << res.construct() << std::endl;

    return 0;
}