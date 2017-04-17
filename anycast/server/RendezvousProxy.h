/**
 * @Author: Michel Andy <andy>
 * @Date:   2017-04-01T10:19:51-04:00
 * @Email:  Andymic12@gmail.com
 * @Project: Anycast
 * @Filename: RAP.h
 * @Last modified by:   andy
 * @Last modified time: 2017-04-17T02:54:20-04:00
 */

 #include "../common/TCPSocket.h"
 #include "../common/Packet.h"
 #include "../common/Utils.h"
 #include "Server.h"
 #include<string>
 #include<map>

class RendezvousProxy : public Server{
private:
    const std::string type = "RAP";
    std::map<int, Node> rap, ingress, join, target;
    Node getClosestJoin(std::string ip, int port, int &distance);
    int getIngressId(std::string ip, int port);
public:
    RendezvousProxy(const char *ip, int port);
    std::string getProxyType();
    int forwardToJoin(Packet * packet);
    bool loadNetConfig(std::string path, NodeType type);
    void printNetConfigs();
    virtual ~RendezvousProxy(){};
};


int main(int argc, char const *argv[]) {

     RendezvousProxy * rvp = new RendezvousProxy(argv[1], atoi(argv[2]));
     rvp->loadNetConfig("../netconfig/rap.conf", RAP);
     rvp->loadNetConfig("../netconfig/target.conf", TARGET);
     rvp->loadNetConfig("../netconfig/join.conf", JOIN);
     rvp->loadNetConfig("../netconfig/ingress.conf", INGRESS);
     //rvp->printNetConfigs();


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
            //  packet->destination_ip = "127.0.0.1";
            //  packet->destination_port = 6001;

             rvp->forwardToJoin(packet);
         }
         delete sock;
         delete packet;
     }
     delete rvp;
     return 0;
 }
