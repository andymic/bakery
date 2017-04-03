/**
 * @Author: Michel Andy <andy>
 * @Date:   2017-03-19T11:49:09-04:00
 * @Email:  Andymic12@gmail.com
 * @Project: Anycast
 * @Filename: TCPConnector.cc
 * @Last modified by:   andy
 * @Last modified time: 2017-04-02T19:53:42-04:00
 */



#include "TCPConnector.h"
#include<iostream>
#include<arpa/inet.h>
#include<unistd.h>
#include<stdlib.h>
#define TAG "TCPConnector - \n"
TCPConnector::TCPConnector(const char *_ip, int _port) : ip(_ip), port(_port){
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    memset((char *) &address, '\0', sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(ip);
    address.sin_port = htons(this->port);
    std::cout<<TAG<<"\t +socket descriptor is "<<sockfd<<std::endl;
}

void TCPConnector::bind(){
    if(sockfd < 0){
        std::cerr <<TAG<<"\t +could not open socket" << std::endl;
        exit(EXIT_FAILURE);
    }

    int optval = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof optval);

    if(::bind(sockfd, (struct sockaddr *) &address, sizeof(address)) < 0){
        std::cerr<<TAG<<"\t +could not bind to socket"<<std::endl;
        close(sockfd);
        exit(EXIT_FAILURE);
    }
}
