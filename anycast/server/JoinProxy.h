/**
 * @Author: Michel Andy <andy>
 * @Date:   2017-04-01T09:56:07-04:00
 * @Email:  Andymic12@gmail.com
 * @Project: Anycast
 * @Filename: JoinProxy.h
 * @Last modified by:   andy
 * @Last modified time: 2017-04-17T03:08:31-04:00
 */

 #include "../common/TCPSocket.h"
 #include "../common/Packet.h"
 #include "../common/Utils.h"
 #include "Server.h"
 #include<map>
 #include<string>

class JoinProxy : public Server{
private:
    const std::string type = "JAP";
    std::string proxy_id;
    std::map<int, Node> targets, joins;
    Node findLocalTarget();
public:
    JoinProxy(const char *ip, int port);
    std::string getProxyType();
    bool loadNetConfig(std::string path, NodeType type);
    int forwardToTarget(Packet **packet);
    int reply(Packet *packet);
    virtual ~JoinProxy(){};
};


 int main(int argc, char const *argv[]) {

     JoinProxy * jap = new JoinProxy(argv[1], atoi(argv[2]));
     jap->loadNetConfig("../netconfig/join.conf", JOIN);
     jap->loadNetConfig("../netconfig/target.conf", TARGET);
     jap->poll();

     while(true){
         TCPSocket* sock = jap->accept();
         int len;
         Packet *packet = NULL;
         len = sock->receive(&packet, 1024);
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
             len = jap->forwardToTarget(&packet);

             if(len > 0){
                 std::cout<<"JAP - \n\t+ got reply from local target:["<<
                 packet->data<<"]\n";
                 packet->proxy_type = "JAP";
                packet->source_port = 6017;
                 jap->reply(packet);
                 break;
             }

         }
         delete sock;
         delete packet;
     }
     delete jap;
     std::cout<<"JAP - \n\t+shuting down...\n";
     return 0;
 }
