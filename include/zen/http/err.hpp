#include<iostream>
#include<stdexcept>

class HttpErr: public std::runtime_error{
public:
    int http_err_code;
    HttpErr(const int& code, const std::string& msg = ""):std::runtime_error("Http error: " + msg), http_err_code(code){};
};