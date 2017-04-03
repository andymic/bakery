/**
 * @Author: Michel Andy <andy>
 * @Date:   2017-04-01T10:16:41-04:00
 * @Email:  Andymic12@gmail.com
 * @Project: Anycast
 * @Filename: IngressProxy.h
 * @Last modified by:   andy
 * @Last modified time: 2017-04-02T19:58:24-04:00
 */

 #include "../common/TCPSocket.h"
 #include "../common/Packet.h"
 #include "Server.h"
 #include<string>

class IngressProxy : public Server{
private:
    const std::string type = "IAP";

public:
    IngressProxy(const char *ip, int port);
    std::string getProxyType();
    int forwardToRap(Packet *packet);
    int forwardToClient(Packet *packet);
    int reply(Packet *packet);
};


int main(int argc, char const *argv[]) {

     IngressProxy * igp = new IngressProxy(argv[1], atoi(argv[2]));
     igp->poll();

     while(true){
         TCPSocket* sock = igp->accept();
         int len;
         Packet *packet = NULL;
         len = sock->receive(&packet, 512);
         if(len > 0){
             if(packet == NULL)
                 std::cout<<"IAP - \n \t+Message received from client [could not decrypt packet]"<<std::endl;
             else
                 std::cout<<"IAP - \n \t+Message received from client ["<<packet->source_ip
                 <<":"<<packet->source_port<<"]"<<" size: "<<len<<std::endl;

             len = 0;
         }

         if(packet->proxy_type == "Client"){
             //packet came from client then forward to RAPProxy
             packet->proxy_type = "IAP";
             packet->forwarder_ip = igp->getIp();
             packet->forwarder_port = igp->getPort();
             packet->destination_ip = "127.0.0.1";
             packet->destination_port = 6002;
             igp->forwardToRap(packet);
         }else if(packet->proxy_type == "JAP"){
              packet->proxy_type = "IAP";
              packet->source_port = 6004;
              igp->forwardToClient(packet);
         }
         delete sock;
         delete packet;

     }

     return 0;
 }
