/**
 * @Author: Michel Andy <andy>
 * @Date:   2017-03-19T11:27:58-04:00
 * @Email:  Andymic12@gmail.com
 * @Project: Anycast
 * @Filename: Server.cc
 * @Last modified by:   andy
 * @Last modified time: 2017-03-20T22:57:22-04:00
 */

#include "Server.h"
#include<iostream>
#include "../common/TCPSocket.h"
#include<arpa/inet.h>
#include<stdlib.h>
#include<cstdlib>
#define TAG "Server - \n"

Server::Server(const char *_ip, int _port) : TCPConnector(_ip, _port){

}

void Server::listen(){
    ::listen(sockfd, 5);
}

TCPSocket* Server::accept(){
    socklen_t  addr_size = sizeof(address);

    newsockfd = ::accept(sockfd, (struct sockaddr*)&address, (socklen_t*) &addr_size);
    if(newsockfd < 0){
        std::cerr<<TAG<<"\t +failed to accept connection\n";
        std::exit(EXIT_FAILURE);
    }
    std::cout<<TAG<<"\t +accepting connections...new socket descriptor is: "<<newsockfd<<std::endl;
    return new TCPSocket(newsockfd, &address);
}

TCPSocket* Server::connect(){
    bind();
    listen();
    return NULL;
}
