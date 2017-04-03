/**
 * @Author: Michel Andy <andy>
 * @Date:   2017-04-01T09:56:07-04:00
 * @Email:  Andymic12@gmail.com
 * @Project: Anycast
 * @Filename: JoinProxy.h
 * @Last modified by:   andy
 * @Last modified time: 2017-04-02T19:23:58-04:00
 */

 #include "../common/TCPSocket.h"
 #include "../common/Packet.h"
 #include "Server.h"
 #include<string>

class JoinProxy : public Server{
private:
    const std::string type = "JAP";

public:
    JoinProxy(const char *ip, int port);
    std::string getProxyType();
    int forwardToTarget(Packet *packet);
    int reply(Packet *packet);
};


 int main(int argc, char const *argv[]) {

     JoinProxy * jap = new JoinProxy(argv[1], atoi(argv[2]));
     jap->poll();

     while(true){
         TCPSocket* sock = jap->accept();
         int len;
         Packet *packet = NULL;
         len = sock->receive(&packet, 512);
         while(len > 0){
             if(packet == NULL)
                 std::cout<<"JAP - \n \t+Message received from client [could not decrypt packet]"<<std::endl;
             else
                 std::cout<<"JAP - \n \t+Message received from client size: "<<len<<std::endl;

             len = 0;
         }
         //Since we won't reply to the RAP (forwarder fields are not overritten), we know that
         //the forwarder info in the packet must have Ingress Proxy's data

         if(packet->proxy_type == "RAP"){
             //if packet came from RAP then forward to target
             packet->proxy_type = "JAP";
             packet->destination_ip = "127.0.0.1";
             packet->destination_port = 6000;
             packet->forwarder_ip = "127.0.0.1";
             packet->forwarder_port = 6001;
             jap->forwardToTarget(packet);
         }else if(packet->proxy_type == "TAP"){
             //if packet came from TAP then reply to Ingress directly
             packet->proxy_type = "JAP";
            //  packet->destination_ip = packet->source_ip;
            //  packet->destination_port = packet->source_port;
            //  packet->source_ip = "127.0.0.1";
            //  packet->source_port = 60001;
             jap->reply(packet);
         }

         delete sock;
         delete packet;
     }

     return 0;
 }
