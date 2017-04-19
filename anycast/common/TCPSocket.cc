/**
 * @Author: Michel Andy <andy>
 * @Date:   2017-03-18T23:07:07-04:00
 * @Email:  Andymic12@gmail.com
 * @Project: Anycast
 * @Filename: TCPSocket.cc
 * @Last modified by:   andy
 * @Last modified time: 2017-04-19T10:53:19-04:00
 */
#include "TCPSocket.h"
#include<iostream>
#include<arpa/inet.h>
#include<unistd.h> //read & write g++
#include<sys/types.h>
#include<string>
#define TAG "TCPSocket - \n"

TCPSocket::TCPSocket(int _sockfd, struct sockaddr_in *_addr, bool _verbose){
    verbose = _verbose;
    char _ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(_addr->sin_addr), _ip, INET_ADDRSTRLEN);
    ip = _ip;
    sockfd = _sockfd;
    port = ntohs(_addr->sin_port);
    if(verbose)
        std::cout<<TAG<<"\t +new TCPSocket created\n";
}

ssize_t TCPSocket::send(const char *buffer, const int len){
    if(verbose)
        std::cout<<TAG<<"\t +sending packet of size "<<len<<std::endl;
    return ::send(sockfd, buffer, len, 0);
}

ssize_t TCPSocket::receive(char *buffer, const int len){
    return ::recv(sockfd, buffer, len, MSG_WAITALL);
}

ssize_t TCPSocket::send(const Packet *packet, const int len){
    if(verbose)
        std::cout<<TAG<<"\t +sending packet of size "<<len<<std::endl;
    return ::send(sockfd, &packet, len, 0);
}

ssize_t TCPSocket::receive(Packet **packet, const int len){
    char cbuf[len];
    ssize_t size = ::recv(sockfd, (char*)&cbuf[0], len, 0);
    std::string buf(cbuf);
    buf.resize(size);
    *packet = new Packet(buf);
    return size;
}

struct EndPointInfo TCPSocket::getEndPointInfo(){
    EndPointInfo info = {ip, port};
    return info;
}

TCPSocket::~TCPSocket(){
    close(sockfd);
}
