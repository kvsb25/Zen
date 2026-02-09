#include <stdexcept>
#include <stacktrace>

class AppErr : public std::runtime_error{
private:
    std::stacktrace trace;
public:
    AppErr(const std::string& msg):std::runtime_error("Middleware Error: "+msg), trace(std::stacktrace::current()){};

    const std::stacktrace& getStackTrace() const noexcept{
        return trace;
    }

    const std::string stackTrace() const noexcept{
        std::stringstream ss;

        ss << "Middleware Error: Stack trace: " << trace << std::endl;
        return ss.str();
    }
};