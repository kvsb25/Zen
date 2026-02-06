#include "../../include/zen/app/app.hpp"

void Zen::handle(http::Request& req, http::Response& res, int index){
    
    if(index >= pipe.size()) return;

    middleware::Middleware* mw = pipe[index].get(); // create a borrowed raw pointer of the unique_ptr at 'index' in pipe // DON'T DELETE mw MANUALLY

    if(mw->type == middleware::Type::DEFAULT){
        try{
            auto* dmw = static_cast<middleware::DefaultMiddleware*>(mw);  // down cast
            dmw->handler(req, res);
            return handle(req, res, index+1);
        } catch (std::runtime_error& err) {
            // handle error
        }
    } else if(mw->type == middleware::Type::PATH) {
        try{
            auto* pmw = static_cast<middleware::PathMiddleware*>(mw);    // down cast
            if(pmw->match(req)){
                pmw->handler(req, res);
                return;
            }
        } catch (std::runtime_error& err){
            // handle error
        }
    }
    // delete all pointers on error 

    return;
}

// Zen::Zen(int port): server(port){}
Zen::Zen() {}

Zen& Zen::use(std::function<void(http::Request&, http::Response&)> callback){
    auto mw = std::make_unique<middleware::DefaultMiddleware>(callback);
    pipe.push_back(std::move(mw));
    return *this;
}

Zen& Zen::use(std::string method, std::string path, std::function<void(http::Request&, http::Response&)> callback){
    auto mw = std::make_unique<middleware::PathMiddleware>(method, path, callback);
    pipe.push_back(std::move(mw));
    return *this;
}

void Zen::listen(const u_short& port, std::function<void(void)> callback){
    TcpServer server(port);
    callback();

    while(true){
        SOCKET client_socket = INVALID_SOCKET;
        client_socket = accept(server.getMainSocket(), NULL, NULL);
        
        ClientSession cs(client_socket);    
        std::string data = cs.recvFromClient();

        http::Request req(data);
        http::Response res;

        this->handle(req, res);

        cs.sendToClient(res.construct());
    }
}