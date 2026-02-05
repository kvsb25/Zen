#include "./tester.hpp"

int main(){
    Zen app;
    app.listen(8080, [](){std::cout<< "hello world"<< std::endl;});
    return 0;
}