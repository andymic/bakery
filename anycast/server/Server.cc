/**
 * @Author: Michel Andy <andy>
 * @Date:   2017-03-19T11:27:58-04:00
 * @Email:  Andymic12@gmail.com
 * @Project: Anycast
 * @Filename: Server.cc
 * @Last modified by:   andy
 * @Last modified time: 2017-04-17T02:23:00-04:00
 */

#include "Server.h"
#include<iostream>
#include "../common/TCPSocket.h"
#include "../client/Client.h"
#include<arpa/inet.h>
#include<unistd.h>
#include<stdlib.h>
#include<cstdlib>
#include<algorithm>
#define TAG "Server - \n"

Server::Server(const char *_ip, int _port=0) : ip(_ip), port(_port){
    max_connections = 15;
    sockets.resize(max_connections);
    std::fill(sockets.begin(), sockets.end(), 0);

    master_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    memset((char *) &address, '\0', sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = (_port == 0) ? INADDR_ANY : inet_addr(ip);
    address.sin_port = htons(this->port);
    std::cout<<TAG<<"\t + "<<ip<<":"<<port<<" socket descriptor is "<<master_sockfd<<std::endl;

}

void Server::listen(){
    //setting max 5 pending connections
    if(::listen(master_sockfd, 5) < 0){
        std::cerr<<"failed to listen...\n";
        std::exit(EXIT_FAILURE);
    }
}

void Server::bind(){
    if(master_sockfd < 0){
        std::cerr <<TAG<<"\t +could not open socket" << std::endl;
        exit(EXIT_FAILURE);
    }

    int optval = 1;
    setsockopt(master_sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof optval);

    if(::bind(master_sockfd, (struct sockaddr *) &address, sizeof(address)) < 0){
        std::cerr<<TAG<<"\t +could not bind to socket"<<std::endl;
        close(master_sockfd);
        exit(EXIT_FAILURE);
    }
}

TCPSocket* Server::accept(){
    socklen_t  addr_size = sizeof(address);

    new_sockfd = ::accept(master_sockfd, (struct sockaddr*)&address, (socklen_t*) &addr_size);
    if(new_sockfd < 0){
        std::cerr<<TAG<<"\t +failed to accept connection\n";
        std::exit(EXIT_FAILURE);
    }
    std::cout<<TAG<<"\t +accepting connections...new socket descriptor is: "<<new_sockfd<<std::endl;
    return new TCPSocket(new_sockfd, &address);
}

std::string Server::getIp(){
    return ip;
}

int Server::getPort(){
    return port;
}

int Server::forward(const char * tip, int tport, Packet **packet, bool wait){
    try{
        std::cout<<TAG<<"\t+Trying to forward packet to local Target ["<<tip<<":"<<tport<<"]\n";
        Client * client = new Client(tip, tport);
        TCPSocket * sock = client->connect();
        std::cout<<TAG<<"\t+succeffully connected target server\n";
        std::string data = (*packet)->to_string();
        int len = sock->send(data.c_str(), data.length());

        if(len < 0){
            std::cout<<TAG<<"\t+packet dropped... \n";
        }else if((unsigned)len < data.length()){
            std::cerr<<TAG<<"\t+data loss: packet size "<<data.length()
            <<" but transmitted size "<<len<<std::endl;
        }else{
            std::cout<<TAG<<"\t+succeffully sent packet of size "<<len
            <<" to "<<tip<<":"<<tport<<std::endl;
            std::cout<<TAG<<"\t+message ["<<(*packet)->data<<"]\n";
        }

        if(wait){
            std::cout<<TAG<<"\t+ waiting for reply...\n";
            while(true){
                len  = 0;
                len = sock->receive(packet, 1024);

                if(len > 0){
                    std::cout<<TAG<<"\t+received: "<<len<<std::endl;
                    std::cout<<TAG<<" data : \n";
                    std::cout<<TAG<<"\t+message ["<<(*packet)->data<<"]\n";
                    break;
                }
            }
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
