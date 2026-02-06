#include "../../include/zen/tcp/clientSession.hpp"

ClientSession::ClientSession(SOCKET& s) : socket(s), buff(DEFAULT_BUFLEN){}

ClientSession::~ClientSession(){
    if(closesocket(socket) == SOCKET_ERROR) {
        std::cout << "Client Socket close failed with error: "
            << WSAGetLastError() 
            << std::endl;
    }

    buff.clear();
}

std::string ClientSession::recvFromClient(){
    int bytesRecv;
    int totalBytesRecv = 0;
    int contentLen = 0;
    int retry = 5; // replace retry with a timeout strategy
    bool headersReceived = false;
    size_t endOfHeaders = std::string::npos;

    if(buff.empty())
        buff.resize((int)DEFAULT_BUFLEN);

    try{
            
            
            
        // loop to continuously receive data from socket kernel buffer to app session buffer
        while(true){
            if( buff.size() == totalBytesRecv){
                buff.resize(buff.size()*2);
            }

            // hard limit protection
            if(buff.size() > (int)MAX_REQUEST_SIZE){
                std::cout << "Request rejected, too large." << std::endl;
                throw std::runtime_error("Request rejected, too large.");
            }

            // call recv(for client's socket, buff starting mem addr, available space that can be filled, remote entity closes the connection)
            bytesRecv = recv( // recv from winsock
                this->socket, 
                buff.data() + totalBytesRecv, // pointer to buffer to receive incoming data
                buff.size() - totalBytesRecv, // space left on buffer
                0
            );

            // check if all data received 
            if(bytesRecv > 0){
                totalBytesRecv += bytesRecv;
                
                // break the loop when all data is received, data with body if(Content length) else till '\r\n\r\n'
                // this is to prevent the application from hanging when header Connection: Keep-Alive exists in the request

                std::string data(buff.data(), totalBytesRecv); // optimize this, we are building the data string on each iteration which is hindering performance
                
                if(endOfHeaders == std::string::npos){
                    endOfHeaders = data.find("\r\n\r\n");
                }
                
                if(endOfHeaders != std::string::npos && !headersReceived){
                    headersReceived = true;

                    size_t pos = data.find("Content-Length:");
                    if(pos != std::string::npos){
                        pos += strlen("Content-Length:");
                        size_t end = data.find("\r\n", pos);
                        std::string contentLenTemp = data.substr(pos, end-pos);
                        contentLen = (int)std::stoul(contentLenTemp);  
                    }

                    // if headers received and content length == 0
                    if(contentLen == 0){
                        break; // headers complete, no content so stop reading
                    }
                }
                
                if(headersReceived && contentLen > 0){
                    size_t bodyBytes = totalBytesRecv - (endOfHeaders + 4);
                    if(bodyBytes >= contentLen){
                        break;
                    }
                }

                retry = 5;
            } else if(bytesRecv == 0) {
                break;
            } else {
                int err = WSAGetLastError();
                if(err == WSAEWOULDBLOCK){
                    if(retry-- > 0) continue;
                    break;
                    throw std::runtime_error("Request Timeout");
                } else {
                    throw WinsockErr();
                }
            }
        }



    } catch(const WinsockErr& e){
        if(!e.cleaned){
            WSACleanup();
        }
    } catch (const std::runtime_error& e){
        if(e.what() == "Request Timeout"){
            sendToClient("Request Timeout");
        }
        std::cerr << e.what() << std::endl;
    }
    
    return std::string(buff.data(), totalBytesRecv);
}

void ClientSession::sendToClient(const std::string& res){
    int totalSent = 0;
    int length = res.length();
    int retry = 5;

    try {


        while(totalSent < length){
            int bytesSent = send(
                socket,
                res.data()+totalSent,
                length-totalSent,
                0
            );

            if(bytesSent>0){
                totalSent += bytesSent;
                retry = 5;
            } else if (bytesSent == 0) {
                break;
            } else {
                
                int err = WSAGetLastError();

                // retry sending
                if(err == WSAEWOULDBLOCK || err == WSAEINTR || err == WSAEINVAL){
                    if(retry-- > 0) continue;
                    throw std::runtime_error("Retry fail");
                } else {
                    throw WinsockErr();
                }
            }
        }


    } catch(const WinsockErr& e){
        if(!e.cleaned){
            WSACleanup();
        }
    } catch (const std::runtime_error& e){
        std::cerr << e.what() << std::endl;
    }
}

void ClientSession::closeSession(){
    buff.clear();
    if(closesocket(socket) == SOCKET_ERROR) {
        std::cout << "Client Socket close failed with error: "
            << WSAGetLastError() 
            << std::endl;
    }
    socket = INVALID_SOCKET;
}