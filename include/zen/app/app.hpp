#pragma once

#include <iostream>
#include <vector>
#include <functional>
#include "../../../include/zen/http/http.hpp"
#include "../../../include/zen/middleware/middleware.hpp"
#include "../../../include/zen/tcp/tcpServer.hpp"
#include "../../../include/zen/tcp/clientSession.hpp"

#include "utils.hpp"

class Zen{
private:
    std::vector<middleware::Middleware*> pipe;
    // TcpServer server;

private:
    void handle(http::Request& req, http::Response& res, int index = 0);

public:
    // Zen(int port); // define the port at which you want your server to listen
    Zen();
    Zen& use(std::function<void(http::Request&, http::Response&)> callback);
    Zen& use(std::string method, std::string path, std::function<void(http::Request&, http::Response&)> callback);
    void listen(u_short& port, std::function<void(void)> callback);
};