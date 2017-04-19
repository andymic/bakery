/**
 * @Author: Michel Andy <andy>
 * @Date:   2017-03-18T22:38:28-04:00
 * @Email:  Andymic12@gmail.com
 * @Project: Anycast
 * @Filename: TCPSocket.h
 * @Last modified by:   andy
 * @Last modified time: 2017-04-19T10:39:35-04:00
 */
#include<string.h>
#include "Packet.h"
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
    bool verbose;
    // Packet translate_packet(void *packet);
public:
    TCPSocket(int _sockfd, struct sockaddr_in *_addr, bool verbose = false);
    ssize_t send(const char *buffer, const int len);
    ssize_t receive(char *buffer, const int len);
    ssize_t send(const Packet *packet, const int len);
    ssize_t receive(Packet **packet, const int len);
    struct EndPointInfo getEndPointInfo();
    ~TCPSocket();
};
#endif
