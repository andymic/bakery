/**
 * @Author: Michel Andy <andy>
 * @Date:   2017-04-02T18:02:10-04:00
 * @Email:  Andymic12@gmail.com
 * @Project: Anycast
 * @Filename: AnycastClient.cc
 * @Last modified by:   andy
 * @Last modified time: 2017-04-19T11:14:03-04:00
 */


#include "Client.h"
#include "../common/TCPSocket.h"
#include "../common/Packet.h"
#include "../common/Utils.h"
#include<vector>
#include<map>
#include<string>

#define TAG "Client - \n\t +"
bool loadIngressConf(std::string path, std::map<int,Node> &ingress_nodes){
    Utils utils;
    bool status = false;
    ingress_nodes = utils.loadConfig(path, INGRESS, status);

    if(!status)
        return false;

    return true;
}

int main(int argc, char const *argv[]) {
    Packet *packet = new Packet();
    std::map<int,Node> ingress_nodes;
    if(loadIngressConf("../netconfig/ingress.conf", ingress_nodes)){
        std::cout<<TAG<<"found "<<ingress_nodes.size()<<" ingress nodes...will send packet from "
        <<ingress_nodes.size()<<" ips"<<std::endl;
        //Response port for the client
        int res_port = 6017; //response port
        Client * resp_client = new Client(argv[1], res_port);
        resp_client->poll();

        for(auto& in : ingress_nodes){
            Client * client = new Client(argv[1], in.second.port);
            TCPSocket *sock = client->connect();
            packet->source_ip = in.second.ip;
            packet->source_port = in.second.port;
            packet->proxy_type = "Client";
            packet->data = "PING";
            packet->hops = 0;
            std::string value = packet->to_string();

            int len = sock->send(value.c_str(), value.length());
            if(len > 0){
                std::cout<<TAG<<"message["<<packet->data<<"] sent successfully\n";
            }

            //In case a process is hangs or packet is dropp...times out after 5 ticks
            int timeout = 3;
            while(timeout > 0){
                TCPSocket*  resp_sock = resp_client->accept();
                len = resp_sock->receive(&packet, 512);
                if(len > 0 && packet->proxy_type == "JAP"){
                    //request served and is forwarded from TAP to JAP  to Client
                    std::cout<< "\033[32m" <<TAG<<"reply received from TAP ["<<
                    packet->data<<"]"<< " hops: "<<packet->hops<<std::endl;
                    std::cout<<"\033[39m";
                    break;
                }
                timeout--;
            }
            delete sock;
            delete client;
        }
    }else{
        std::cerr<<TAG<<"could not load ingress ips\n";
    }

    delete packet;

    return 0;
}
