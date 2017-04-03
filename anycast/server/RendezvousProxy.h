/**
 * @Author: Michel Andy <andy>
 * @Date:   2017-04-01T10:19:51-04:00
 * @Email:  Andymic12@gmail.com
 * @Project: Anycast
 * @Filename: RAP.h
 * @Last modified by:   andy
 * @Last modified time: 2017-04-02T19:24:15-04:00
 */

 #include "../common/TCPSocket.h"
 #include "../common/Packet.h"
 #include "Server.h"
 #include<string>

class RendezvousProxy : public Server{
private:
    const std::string type = "RAP";
public:
    RendezvousProxy(const char *ip, int port);
    std::string getProxyType();
    int forwardToJoin(Packet * packet);
};


int main(int argc, char const *argv[]) {

     RendezvousProxy * rvp = new RendezvousProxy(argv[1], atoi(argv[2]));
     rvp->poll();

     while(true){
         TCPSocket* sock = rvp->accept();
         int len;
         Packet *packet = NULL;
         len = sock->receive(&packet, 512);
         if(len > 0){
             if(packet == NULL)
                 std::cout<<"RAP - \n \t+Message received from client [could not decrypt packet]"<<std::endl;
             else
                 std::cout<<"IAP - \n \t+Message received from client ["<<packet->source_ip
                 <<":"<<packet->source_port<<"]"<<" size: "<<len<<std::endl;

             len = 0;
         }

         if(packet->proxy_type == "IAP"){
             //packet is coming from client
             packet->proxy_type = "RAP";
             packet->destination_ip = "127.0.0.1";
             packet->destination_port = 6001;

             rvp->forwardToJoin(packet);
         }
         delete sock;
         delete packet;
     }

     return 0;
 }
