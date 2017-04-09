/**
 * @Author: Michel Andy <andy>
 * @Date:   2017-03-19T11:27:58-04:00
 * @Email:  Andymic12@gmail.com
 * @Project: Anycast
 * @Filename: Server.cc
 * @Last modified by:   andy
 * @Last modified time: 2017-04-09T01:23:40-04:00
 */

#include "Server.h"
#include<iostream>
#include "../common/TCPSocket.h"
#include "../client/Client.h"
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

std::string Server::getIp(){
    return ip;
}

int Server::getPort(){
    return port;
}

int Server::forward(const char * tip, int tport, Packet *packet){
    try{
        std::cout<<TAG<<"\t+Trying to forward packet to local Target ["<<tip<<":"<<tport<<"]\n";
        Client * client = new Client(tip, tport);
        TCPSocket * sock = client->connect();
        std::cout<<TAG<<"\t+succeffully connected target server\n";
        std::string data = packet->to_string();
        int len = sock->send(data.c_str(), data.length());

        if(len < 0){
            std::cout<<TAG<<"\t+packet dropped... \n";
        }else if((unsigned)len < data.length()){
            std::cerr<<TAG<<"\t+data loss: packet size "<<data.length()
            <<" but transmitted size "<<len<<std::endl;
        }else{
            std::cout<<TAG<<"\t+succeffully sent packet of size "<<len
            <<" to "<<tip<<":"<<tport<<std::endl;
            std::cout<<TAG<<"\t+message ["<<packet->data<<"]\n";
        }
        delete sock;
        delete client;
        return len;
    }catch(std::exception &e ){
        std::cout<<TAG<<"\t+"<<e.what()<<std::endl;
    }
    return 0;
}
bool Server::poll(){
    bind();
    listen();
    return true;
}

Server::~Server(){

}
