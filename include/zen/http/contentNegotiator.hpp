#pragma once
#include "utils.hpp"

namespace http{
    class ContentNegotiator{
    public:
        static FormatPriority parse(const std::string& header_val);
        static std::string match(const FormatHandlerMap& handler_map, const FormatPriority& accepted_val);
    };
}