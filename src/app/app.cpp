#include "../../include/zen/app/app.hpp"

void Zen::handle(http::Request& req, http::Response& res, int index = 0){
    
    if(index >= pipe.size()) return;

    middleware::Middleware* mw = pipe[index];

    if(mw->type == middleware::Type::DEFAULT){
        try{
            middleware::DefaultMiddleware* dmw = static_cast<middleware::DefaultMiddleware*>(mw);  // down cast
            dmw->handler(req, res);
            return handle(req, res, index+1);
        } catch (std::runtime_error& err) {
            // handle error
        }
    } else if(mw->type == middleware::Type::PATH) {
        try{
            middleware::PathMiddleware* pmw = static_cast<middleware::PathMiddleware*>(mw);    // down cast
            if(pmw->match(req)){
                pmw->handler(req, res);
                return;
            }
        } catch (std::runtime_error& err){
            // handle error
        }
    }

    return;
}

// Zen::Zen(int port): server(port){}
Zen::Zen() {}

Zen& Zen::use(std::function<void(http::Request&, http::Response&)> callback){
    middleware::DefaultMiddleware* mw = new middleware::DefaultMiddleware(callback);
    pipe.push_back(mw);
}

Zen& Zen::use(std::string method, std::string path, std::function<void(http::Request&, http::Response&)> callback){
    middleware::PathMiddleware* mw = new middleware::PathMiddleware(method, path, callback);
    pipe.push_back(mw);
}

void Zen::listen(const u_short& port, std::function<void(void)> callback){
    TcpServer server(port);
    callback();

    while(true){
        SOCKET client_socket = INVALID_SOCKET;
        client_socket = accept(server.getMainSocket(), NULL, NULL);

        ClientSession cs(client_socket);

        std::string data = cs.recvFromClient();

        http::Request* req = new http::Request(data);
        http::Response* res = new http::Response();

        this->handle(*req, *res);

        cs.sendToClient(res->construct());
        cs.closeSession();
    }
}