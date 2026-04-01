#pragma once
#include "utils.hpp"

namespace http{
    class ContentNegotiator{
    public:
        static FormatPriority parser(std::string header_val);
        static std::string match(FormatHandlerMap handler_map, FormatPriority accepted_val);
    };
}