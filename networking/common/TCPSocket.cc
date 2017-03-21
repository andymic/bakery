/**
 * @Author: Michel Andy <andy>
 * @Date:   2017-03-18T23:07:07-04:00
 * @Email:  Andymic12@gmail.com
 * @Project: Anycast
 * @Filename: TCPSocket.cc
 * @Last modified by:   andy
 * @Last modified time: 2017-03-20T23:14:01-04:00
 */
#include "TCPSocket.h"
#include<iostream>
#include<arpa/inet.h>
#include<unistd.h> //read & write g++
#include<sys/types.h>
#include<string>
#define TAG "TCPSocket - \n"

TCPSocket::TCPSocket(int _sockfd, struct sockaddr_in *_addr){
    char _ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(_addr->sin_addr), _ip, INET_ADDRSTRLEN);
    ip = _ip;
    sockfd = _sockfd;
    port = ntohs(_addr->sin_port);
    std::cout<<TAG<<"\t +new TCPSocket created\n";
}

ssize_t TCPSocket::send(const char *buffer, const int len){
    std::cout<<TAG<<"\t +sending something of size "<<len<<std::endl;
    return write(sockfd, buffer, len);
}

ssize_t TCPSocket::receive(char *buffer, const int len){
    return read(sockfd, buffer, len);
}

struct EndPointInfo TCPSocket::getEndPointInfo(){
    EndPointInfo info = {ip, port};
    return info;
}

TCPSocket::~TCPSocket(){
    close(sockfd);
}
