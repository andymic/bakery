/**
 * @Author: Michel Andy <andy>
 * @Date:   2017-03-19T11:13:44-04:00
 * @Email:  Andymic12@gmail.com
 * @Project: Anycast
 * @Filename: Server.h
 * @Last modified by:   andy
 * @Last modified time: 2017-03-20T23:14:21-04:00
 */

#include "../common/TCPConnector.h"
#include "../common/TCPSocket.h"
#include<iostream>
// #include <netinet/in.h>
#include <stdlib.h>

class Server: public TCPConnector {
private:
    int newsockfd;
    void listen();
public:
    Server(const char *ip, int port);
    TCPSocket* connect();
    TCPSocket* accept();
};

int main(int argc, char const *argv[]) {
    /* code */

    Server * srv = new Server(argv[1], atoi(argv[2]));
    srv->connect();
    std::string response = "I got your message";
    while(true){
        TCPSocket* sock = srv->accept();
        int len;
        char message [256];
        memset(message, '\0', 256);
        while((len = sock->receive(message, sizeof(message))) > 0){
            std::cout<<"Server - \n \t+Message received from client ["<<message<<"]"<<std::endl;
            sock->send(response.c_str(), response.length());
        }
        delete sock;
    }
    return 0;
}
