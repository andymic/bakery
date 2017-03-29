/**
 * @Author: Michel Andy <andy>
 * @Date:   2017-03-19T15:54:27-04:00
 * @Email:  Andymic12@gmail.com
 * @Project: Anycast
 * @Filename: Client.h
 * @Last modified by:   andy
 * @Last modified time: 2017-03-28T20:32:15-04:00
 */
#include "../common/TCPConnector.h"
#include "../common/TCPSocket.h"
#include "../common/Packet.h"
#include<iostream>
#include<string>
#include<netdb.h>
#include <stdlib.h>
#include <sstream>

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


class Client : public TCPConnector{
private:
    struct hostent *server;
public:
    Client(const char *ip, int port);
    TCPSocket* connect();
    ~Client();
};

int main(int argc, char const *argv[]) {
    Client * client = new Client(argv[1], atoi(argv[2]));
    TCPSocket *sock = client->connect();
    Packet *packet = new Packet();
    packet->data = "test from client";
    std::string value = packet->to_string();

    int len = sock->send(value.c_str(), value.length());
    if(len > 0){
        std::cout<<"Client - \n\t +message sent successfully\n";
    }

    delete sock;
    return 0;
}
