/**
 * @Author: Michel Andy <andy>
 * @Date:   2017-03-19T10:39:44-04:00
 * @Email:  Andymic12@gmail.com
 * @Project: Anycast
 * @Filename: TCPConnector.h
 * @Last modified by:   andy
 * @Last modified time: 2017-03-19T12:30:08-04:00
 */

#include "TCPSocket.h"
#include<netdb.h>
#include<netinet/in.h>

#ifndef __tcpconnector__h__
#define __tcpsocket__h__

class TCPConnector {
protected:
    const char *ip;
    int port;
    int sockfd;
    struct sockaddr_in address;
public:
    TCPConnector(const char * _ip, int _port);
    virtual TCPSocket* connect() = 0;
    void bind();
};
#endif
