#include "./tester.hpp"

int main(){
    Zen app;
    
    app.use([](http::Request& req, http::Response& res){
        std::cout << "This is a default middleware" << std::endl;
        std::cout << "res.body: " << res.body << std::endl;
        return;
    });

    app.use("GET", "/vendor/product/:id", [](http::Request& req, http::Response& res){
        std::string html = 
            "<!DOCTYPE html>\n"
            "<html lang=\"en\">\n"
            "<head>\n"
            "    <meta charset=\"UTF-8\">\n"
            "    <title>Product Test</title>\n"
            "</head>\n"
            "<body>\n"
            "    <h1>Product ID: "+req.params["id"]+"</h1>\n"
            "    <p>this is product you asked for</p>\n"
            "</body>\n"
            "</html>";
        
        res.send(html);
        return;
    });

    app.use("POST", "/vendor/product", [](http::Request& req, http::Response& res){
        std::string html = 
            "<!DOCTYPE html>\n"
            "<html lang=\"en\">\n"
            "<head>\n"
            "    <meta charset=\"UTF-8\">\n"
            "    <title>Product Test</title>\n"
            "</head>\n"
            "<body>\n"
            "    <h1>Product has been posted</h1>\n"
            "</body>\n"
            "</html>";

        res.send(html);
        return;
    });

    app.listen(8080, [](){std::cout<< "hello world"<< std::endl;});
    return 0;
}