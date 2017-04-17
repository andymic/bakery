/**
 * @Author: Michel Andy <andy>
 * @Date:   2017-04-02T18:02:10-04:00
 * @Email:  Andymic12@gmail.com
 * @Project: Anycast
 * @Filename: AnycastClient.cc
 * @Last modified by:   andy
 * @Last modified time: 2017-04-16T19:43:42-04:00
 */


#include "Client.h"
#include "../common/TCPSocket.h"
#include "../common/Packet.h"

int main(int argc, char const *argv[]) {
    Client * client = new Client(argv[1], atoi(argv[2]));
    TCPSocket *sock = client->connect();
    Packet *packet = new Packet();
    packet->source_ip = argv[1];
    packet->source_port = atoi(argv[2]);
    packet->proxy_type = "Client";
    packet->data = "I made tea";
    std::string value = packet->to_string();

    int len = sock->send(value.c_str(), value.length());
    if(len > 0){
        std::cout<<"Client - \n\t +message["<<packet->data<<"] sent successfully\n";
    }
    int res_port = 6004; //response port
    client = new Client(argv[1], res_port);
    client->poll();
    while(true){
        sock = client->accept();
        len = sock->receive(&packet, 512);
        if(len > 0 && packet->proxy_type == "JAP"){
            //request served and is forwarded from TAP to JAP  to Client
            std::cout<<"Client - \n\t +reply received from TAP ["<<
            packet->data<<"]"<< "hops: "<<packet->hops<<std::endl;
            break;
        }
    }
    delete packet;
    delete sock;
    delete client;
    return 0;
}
