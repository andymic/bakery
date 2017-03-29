/**
 * @Author: Michel Andy <andy>
 * @Date:   2017-03-19T11:13:44-04:00
 * @Email:  Andymic12@gmail.com
 * @Project: Anycast
 * @Filename: Server.h
 * @Last modified by:   andy
 * @Last modified time: 2017-03-28T20:03:56-04:00
 */

#include "../common/TCPConnector.h"
#include "../common/TCPSocket.h"
#include "../common/Packet.h"
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

    while(true){
        TCPSocket* sock = srv->accept();
        int len;
        Packet *packet = NULL;
        len = sock->receive(&packet, 512);
        while(len > 0){
            if(packet == NULL)
                std::cout<<"Server - \n \t+Message received from client [could not decrypt packet]"<<std::endl;
            else
                std::cout<<"Server - \n \t+Message received from client ["<<packet->data<<"]"<<"size: "<<len<<std::endl;

            len = 0;
            break;

        }
        delete sock;
        delete packet;
    }

    return 0;
}
