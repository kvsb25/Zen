#pragma once
#include <stdexcept>
#include <stacktrace>
#include "utils.hpp"

class ZenErr : public std::runtime_error{
private:
    std::stacktrace trace;
    bool is_relevant;
public:
    ZenErr(const std::string& msg = "", bool flag = true):std::runtime_error("Zen Error: " + msg), trace(std::stacktrace::current()), is_relevant(flag) {};

    inline const bool isRelevant() const{
        return is_relevant;
    }

    inline const std::string getTrace() const {
        std::stringstream ss;
        ss << "Zen Error: Stack trace: " << trace << std::endl;

        return ss.str();
    }
};