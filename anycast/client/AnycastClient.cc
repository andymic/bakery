/**
 * @Author: Michel Andy <andy>
 * @Date:   2017-04-02T18:02:10-04:00
 * @Email:  Andymic12@gmail.com
 * @Project: Anycast
 * @Filename: AnycastClient.cc
 * @Last modified by:   andy
 * @Last modified time: 2017-04-19T11:42:43-04:00
 */


#include "Client.h"
#include "../common/TCPSocket.h"
#include "../common/Packet.h"
#include "../common/Utils.h"
#include<iostream>
#include<vector>
#include<map>
#include<string>
#include<fstream>

#define TAG "Client - \n\t +"
struct HOP{
    std::string client;
    int hops;
};

bool loadIngressConf(std::string path, std::map<int,Node> &ingress_nodes){
    Utils utils;
    bool status = false;
    ingress_nodes = utils.loadConfig(path, INGRESS, status);

    if(!status)
        return false;

    return true;
}

void log (std::string path, std::vector<HOP> hops, int proxy_count, int req_per_minute){
    std::ofstream fs(path, std::ios::out);

    if(fs.is_open()){
        int total = 0;
        fs <<"Anycast Server Log\n";
        fs <<"~~~~~~~~~~~~~~~~~~\n";
        for(auto& h: hops){
            fs<<h.client<<"="<<h.hops<<std::endl;
            total +=h.hops;
        }
        fs << "Average cost per minute: "<<total/req_per_minute<<std::endl;
        fs.close();
    }else{
      std::cerr<<TAG<<"could not create file: "<<path<<std::endl;
    }
}

int main(int argc, char const *argv[]) {
    Packet *packet = new Packet();
    std::map<int,Node> ingress_nodes;
    std::vector<HOP> hops;
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
                    HOP hop = {in.second.ip + ":" + std::to_string(in.second.port), packet->hops};
                    hops.push_back(hop);
                    break;
                }
                timeout--;
            }
            delete sock;
            delete client;
        }
        log("../anycast_sim.log", hops, ingress_nodes.size(), 1);
    }else{
        std::cerr<<TAG<<"could not load ingress ips\n";
    }

    delete packet;

    return 0;
}
