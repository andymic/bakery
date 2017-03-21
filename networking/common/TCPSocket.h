/**
 * @Author: Michel Andy <andy>
 * @Date:   2017-03-18T22:38:28-04:00
 * @Email:  Andymic12@gmail.com
 * @Project: Anycast
 * @Filename: TCPSocket.h
 * @Last modified by:   andy
 * @Last modified time: 2017-03-20T23:13:54-04:00
 */
#include<string.h>
#include <sys/types.h>
#include <sys/socket.h>

#ifndef __tcpsocket__h__
#define __tcpsocket__h__

struct EndPointInfo {
    const char *ip;
    int port;
};

class TCPSocket {
private:
    int sockfd;
    const char *ip;
    int port;
public:
    TCPSocket(int _sockfd, struct sockaddr_in *_addr);
    ssize_t send(const char *buffer, const int len);
    ssize_t receive(char *buffer, const int len);
    struct EndPointInfo getEndPointInfo();
    ~TCPSocket();
};
#endif
