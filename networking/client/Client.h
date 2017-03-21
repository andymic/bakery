/**
 * @Author: Michel Andy <andy>
 * @Date:   2017-03-19T15:54:27-04:00
 * @Email:  Andymic12@gmail.com
 * @Project: Anycast
 * @Filename: Client.h
 * @Last modified by:   andy
 * @Last modified time: 2017-03-20T23:14:40-04:00
 */
#include "../common/TCPConnector.h"
#include "../common/TCPSocket.h"
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

    std::string diff="test from client";

    int len = sock->send(diff.c_str(), diff.length());
    if(len > 0){
        std::cout<<"Client - \n\t +message sent successfully\n";
    }

    char message[256];
    memset(message, '\0', 256);

    while((len = sock->receive(message, sizeof(message))) > 0){
        std::cout<<"Client - \n\t +message received from server ["<<message<<"]"<<std::endl;
    }

    delete sock;
    return 0;
}
