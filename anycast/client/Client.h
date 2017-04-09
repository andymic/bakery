/**
 * @Author: Michel Andy <andy>
 * @Date:   2017-03-19T15:54:27-04:00
 * @Email:  Andymic12@gmail.com
 * @Project: Anycast
 * @Filename: Client.h
 * @Last modified by:   andy
 * @Last modified time: 2017-04-09T01:18:48-04:00
 */
#include "../common/TCPSocket.h"
#include "../server/Server.h"
#include<iostream>
#include<string>
#include<netdb.h>
#include<stdlib.h>
#include<sstream>
#ifndef __client__h__
#define __client__h__
namespace patch
{
    template <typename T>
    std::string to_string(const T& value)
    {
    	std::ostringstream os;
    	os << value;
    	return os.str();
    }
}


class Client : public Server{
private:
    struct hostent *server;
public:
    Client(const char *ip, int port);
    TCPSocket* connect();
    virtual ~Client();
};
#endif
