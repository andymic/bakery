/**
 * @Author: Michel Andy <andy>
 * @Date:   2017-03-19T15:57:40-04:00
 * @Email:  Andymic12@gmail.com
 * @Project: Anycast
 * @Filename: Client.cc
 * @Last modified by:   andy
 * @Last modified time: 2017-04-16T18:25:39-04:00
 */

#include "Client.h"
#include<iostream>
#include<stdlib.h>
#include<unistd.h>
#define TAG "Client - \n"
Client::Client(const char *_ip, int _port) : Server(_ip, _port){

}

TCPSocket* Client::connect(){
    server = gethostbyname(ip);

    if(server == NULL){
        std::cerr<<TAG<<"\t +could not find host ip\n";
        exit(EXIT_FAILURE);
    }

    if(::connect(master_sockfd, (struct sockaddr *)&address, sizeof(address)) < 0){
        std::cerr<<TAG<<"\t +could not connect host\n";
        exit(EXIT_FAILURE);
    }
    std::cout<<TAG<<"\t +connection establish to host "<<ip<<":"<<port<<std::endl;
    return new TCPSocket(master_sockfd, &address);
}

Client::~Client(){
    close(master_sockfd);
}
