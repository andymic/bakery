/**
 * @Author: Michel Andy <andy>
 * @Date:   2017-04-01T17:00:54-04:00
 * @Email:  Andymic12@gmail.com
 * @Project: Anycast
 * @Filename: Target.h
 * @Last modified by:   andy
 * @Last modified time: 2017-04-02T19:24:25-04:00
 */

 #include "../common/TCPSocket.h"
 #include "../common/Packet.h"
 #include "Server.h"
 #include<string>

 class Target : public Server{
 private:
    const std::string type = "TAP";
 public:
    Target(const char *ip, int port);
    int reply(Packet* packet);
    std::string getProxyType();
 };

 int main(int argc, char const *argv[]) {

     Target * tgt = new Target(argv[1], atoi(argv[2]));
     tgt->poll();

     while(true){
         TCPSocket* sock = tgt->accept();
         int len;
         Packet *packet = NULL;
         len = sock->receive(&packet, 512);
         if(len > 0){
             if(packet == NULL)
                 std::cout<<"TAP - \n \t+Message received from client [could not decrypt packet]"<<std::endl;
             else
                 std::cout<<"TAP - \n \t+Message received from client ["<<packet->forwarder_ip
                 <<":"<<packet->forwarder_port<<"]"<<" size: "<<len<<std::endl;

             len = 0;
         }
         if(packet->proxy_type == "JAP"){
             //packet should only come from the Join Proxy
             packet->proxy_type = "TAP";

             tgt->reply(packet);
         }

         delete sock;
         delete packet;
     }

     return 0;
 }
