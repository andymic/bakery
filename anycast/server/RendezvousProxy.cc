/**
 * @Author: Michel Andy <andy>
 * @Date:   2017-04-01T16:27:20-04:00
 * @Email:  Andymic12@gmail.com
 * @Project: Anycast
 * @Filename: RendezvousProxy.cc
 * @Last modified by:   andy
 * @Last modified time: 2017-04-19T10:49:13-04:00
 */

#include "RendezvousProxy.h"
#include <fstream>
#include <limits.h>
#define TAG "RendezvousProxy-\n\t+"
RendezvousProxy::RendezvousProxy(const char * _ip, int _port, bool _verbose) : Server(_ip, _port, _verbose){
    // JAPInfo jap1 = {6003, }
}

std::string RendezvousProxy::getProxyType(){
    return type;
}

int RendezvousProxy::getIngressId(std::string ip, int port){
    if(verbose)
        std::cout<<TAG<<"looking up ingress by id...\n";

    for(auto& i: ingress){
        if(i.second.ip == ip && i.second.port == port){
            return i.first;
        }
    }
    if(verbose)
        std::cout<<TAG<<"ingress proxy is not part of the network\n";
    return 0;
}
Node RendezvousProxy::getClosestJoin(std::string ip, int port, int &distance){
    if(verbose)
        std::cout<<TAG<<"looking up closest join proxy...\n";

    int ingress_id = getIngressId(ip, port);
    int node_id;
    if(ingress_id != 0){
        int min_distance = INT_MAX;
        for(auto& j: join){
            int d = abs(j.first - ingress_id);
            if(d < min_distance){
                min_distance = d;
                node_id = j.first;
            }
        }
        distance = min_distance;
        if(verbose)
            std::cout<<TAG<<"closest join proxy id: "<<node_id<<std::endl;

        return join.at(node_id);
    }
    Node t ={"", 0};
    return t;
}


int RendezvousProxy::forwardToJoin(Packet *packet){
    Node join;
    int distance = 0;
    //using node_id as the location to calculate distances
    join = getClosestJoin(packet->forwarder_ip, packet->forwarder_port, distance);

    if(join.ip != "" && join.port != 0){
        if(verbose){
            std::cout<<TAG<<"forwarding packet from ["<<packet->source_ip<<":"
            <<packet->source_port<<"] to closest join proxy\n";
        }

        //distance == 0, indicates that the join is local so the packet is forwarded without a cost
        if(distance > 0)
            packet->hops++;

        packet->destination_ip = join.ip;
        packet->destination_port  = join.port;
        return forward(packet->destination_ip.c_str(), packet->destination_port, &packet);
    }

    if(verbose)
        std::cout<<TAG<<"could not find nearest join\n";
    return 0;
}
bool RendezvousProxy::loadNetConfig(std::string path, NodeType type){
    Utils utls;
    bool status = false;
        switch (type) {
            case RAP :
                rap = utls.loadConfig(path, type, status);
                break;
            case INGRESS:
                ingress = utls.loadConfig(path, type, status);
                break;
            case TARGET:
                target = utls.loadConfig(path, type, status);
                break;
            case JOIN:
                join = utls.loadConfig(path, type, status);
                break;
        }
    return (status == true) ? true : false;
}

void RendezvousProxy::printNetConfigs(){
    Utils utls;
    utls.printConfig(rap, RAP);
    utls.printConfig(ingress, INGRESS);
    utls.printConfig(target, TARGET);
    utls.printConfig(join, JOIN);
}
//
// bool RendezvousProxy::loadNetConfig(std::string path, NodeType type){
//     std::map<int, Node> *temp;
//     switch (type) {
//         case RAP :
//             temp = &rap;
//             break;
//         case INGRESS:
//             temp = &ingress;
//             break;
//         case TARGET:
//             temp = &target;
//             break;
//         case JOIN:
//             temp = &join;
//             break;
//     }
//
//     std::ifstream conf_file(path);
//
//     std::string ip;
//     int id, port;
//
//     if(conf_file.is_open()){
//         while(conf_file >> id >> ip >> port){
//             Node n = {ip, port};
//             temp->insert(std::pair<int, Node>(id, n));
//         }
//     }else{
//         std::cerr<<TAG<<"failed to load "<<path<<" config file\n";
//         return false;
//     }
//     std::cout<<"Successfully loaded "<<path<<std::endl;
//
//     return true;
// }
//
// void RendezvousProxy::printConfigs(){
//     std::cout<<TAG<<"Values for RAP nodes\n";
//     std::cout<<"\tNODE_ID IP \t\tPORT\n";
//     for(auto& c: rap){
//         std::cout<<"\t"<<c.first<<"\t"<<c.second.ip<<"\t"<<c.second.port<<std::endl;
//     }
//
//     std::cout<<TAG<<"Values for INGRESS nodes\n";
//     std::cout<<"\tNODE_ID IP \t\tPORT\n";
//     for(auto& c: ingress){
//         std::cout<<"\t"<<c.first<<"\t"<<c.second.ip<<"\t"<<c.second.port<<std::endl;
//     }
//
//     std::cout<<TAG<<"Values for JOIN nodes\n";
//     std::cout<<"\tNODE_ID IP \t\tPORT\n";
//     for(auto& c: join){
//         std::cout<<"\t"<<c.first<<"\t"<<c.second.ip<<"\t"<<c.second.port<<std::endl;
//     }
//
//     std::cout<<TAG<<"Values for TARGET nodes\n";
//     std::cout<<"\tNODE_ID IP \t\tPORT\n";
//     for(auto& c: target){
//         std::cout<<"\t"<<c.first<<"\t"<<c.second.ip<<"\t"<<c.second.port<<std::endl;
//     }
// }
