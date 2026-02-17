#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <functional>
#include "../../../include/zen/http/http.hpp"
#include "../../../include/zen/middleware/middleware.hpp"
#include "../../../include/zen/tcp/tcpServer.hpp"
#include "../../../include/zen/tcp/clientSession.hpp"

#include "utils.hpp"
#include "err.hpp"

class Zen{
private:
    std::vector<std::unique_ptr<middleware::Middleware>> pipe; // try to convert to std::vector<std::unique_ptr<middleware::Middleware>>
    // TcpServer server;

private:
    void handle(http::Request& req, http::Response& res, int index = 0, const ZenErr& err = ZenErr("", false));

public:
    // Zen(int port); // define the port at which you want your server to listen
    Zen();
    Zen& use(std::function<void(http::Request&, http::Response&)> callback);
    Zen& use(std::string method, std::string path, std::function<void(http::Request&, http::Response&)> callback);
    void listen(const u_short& port, std::function<void(void)> callback = [](){});
};