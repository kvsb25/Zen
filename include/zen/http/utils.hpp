#pragma once
#include <iostream>
#include <unordered_map>

namespace http
{
    std::unordered_map<int, std::string> message_for_status = {
        {100, "Continue"},
        {200, "OK"},
        {201, "Created"},
        {204, "No Content"},
        {301, "Moved Permanently"},
        {302, "Found"},
        {400, "Bad Request"},
        {401, "Unauthorized"},
        {403, "Forbidden"},
        {404, "Not Found"},
        {405, "Method Not Allowed"},
        {500, "Internal Server Error"},
        {503, "Service Unavailable"}
    };
}