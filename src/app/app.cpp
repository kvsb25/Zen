#include "../../include/zen/app/app.hpp"

void Zen::handle(http::Request& req, http::Response& res, int index, const ZenErr& err){
    
    if(index >= pipe.size()) return;

    middleware::Middleware* mw = pipe[index].get(); // create a borrowed raw pointer of the unique_ptr at 'index' in pipe // DON'T DELETE mw MANUALLY

    // if(!err.isRelevant() && mw->type == middleware::Type::DEFAULT){
    //     try{
    //         auto* dmw = static_cast<middleware::DefaultMiddleware*>(mw);  // down cast
    //         dmw->handler(req, res);
    //         // give flag -02 or -03 with gcc to leverage tail call optimization
    //         return handle(req, res, index+1, err);

    //     } catch (const ZenErr& e){
    //         return handle(req, res, index+1, e);

    //     } catch (const std::runtime_error& err) {
    //         /*CHANGE ERROR HANDLING FOR ONION ARCH*/ throw HandlerErr(err.what());
    //     }
    // } else if(!err.isRelevant() && mw->type == middleware::Type::PATH) {
    //     try{
    //         auto* pmw = static_cast<middleware::PathMiddleware*>(mw);    // down cast
    //         if(pmw->match(req)){
    //             pmw->handler(req, res);
    //             return;
    //         }
    //     } catch (const ZenErr& e){
    //         return handle(req, res, index+1, e);
    //     } catch (const std::runtime_error& err){
    //         /*CHANGE ERROR HANDLING FOR ONION ARCH*/ throw HandlerErr(err.what());
    //     }
    // } else if(err.isRelevant() && mw->type == middleware::Type::ERR){
    //     try{
    //         auto* emw = static_cast<middleware::ErrorMiddleware*>(mw);
    //         emw->handler(err, req, res);
    //     } catch (...) {
    //         throw HandlerErr(err.what());
    //     }
    // }
    try
    {
        if (!err.isRelevant())
        {
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
        return handle(req, res, index + 1, err);
    }
    catch (const ZenErr &e)
    {
        if(e.isRelevant())
            return handle(req, res, index + 1, e);
    }
    catch (const std::runtime_error& e){
        throw HandlerErr(e.what());
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

    try{


        while(true){
            SOCKET client_socket = INVALID_SOCKET;
            client_socket = accept(server.getMainSocket(), NULL, NULL);
            
            ClientSession cs(client_socket);    
            std::string data = cs.recvFromClient();

            http::Response res;
            try{

            http::Request req(data);

            this->handle(req, res);

            } catch(const HttpErr& e){

                std::cerr << e.what() << std::endl;
                res.initErrRes(e.http_err_code, e.what());
                
            } catch(const HandlerErr& e){
                // std::cerr << e.stackTrace() << std::endl;
                std::cerr << e.what() << std::endl;
                res.initErrRes(500, e.what());
            } catch(const std::runtime_error& e){

                std::cerr << e.what() << std::endl;
                res.initErrRes(500, e.what());

            }
            cs.sendToClient(res.construct());
        }


    } catch (const WinsockErr& e){
        std::cerr<<e.what()<<std::endl;
        if(!e.cleaned){
            WSACleanup();
        }
    } catch (const std::runtime_error& e){
        std::cerr<<e.what()<<std::endl;
    }
}