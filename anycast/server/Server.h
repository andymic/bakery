/**
 * @Author: Michel Andy <andy>
 * @Date:   2017-03-19T11:13:44-04:00
 * @Email:  Andymic12@gmail.com
 * @Project: Anycast
 * @Filename: Server.h
 * @Last modified by:   andy
 * @Last modified time: 2017-04-02T19:50:15-04:00
 */

#include "../common/TCPConnector.h"
#include "../common/TCPSocket.h"
#include "../common/Packet.h"
#include<iostream>
#include<stdlib.h>

#ifndef __server__h__
#define __server__h__

class Server: public TCPConnector {
private:
    int newsockfd;
    void listen();

public:
    Server(const char *ip, int port);
    bool poll();
    TCPSocket* connect(){return NULL;};
    TCPSocket* accept();
    std::string getIp();
    int getPort();
    int forward(const char * tip, int tport, Packet *packet);
    virtual ~Server() = 0;
};

// int main(int argc, char const *argv[]) {
//
//      Server * srv = new Server(argv[1], atoi(argv[2]));
//      srv->connect();
//
//      while(true){
//          TCPSocket* sock = srv->accept();
//          int len;
//          Packet *packet = NULL;
//          len = sock->receive(&packet, 512);
//          while(len > 0){
//              if(packet == NULL)
//                  std::cout<<"JAP - \n \t+Message received from client [could not decrypt packet]"<<std::endl;
//              else
//                  std::cout<<"JAP - \n \t+Message received from client ["<<packet->data<<"]"<<" size: "<<len<<std::endl;
//
//              len = 0;
//              break;
//
//          }
//          delete sock;
//          delete packet;
//      }
//
//      return 0;
//  }
#endif
