/**
 * @Author: Michel Andy <andy>
 * @Date:   2017-04-01T16:29:20-04:00
 * @Email:  Andymic12@gmail.com
 * @Project: Anycast
 * @Filename: IngressProxy.cc
 * @Last modified by:   andy
 * @Last modified time: 2017-04-17T02:34:29-04:00
 */


#include "IngressProxy.h"
#include <limits.h>
#define TAG "IngressProxy-\n\t+"
IngressProxy::IngressProxy(const char * _ip, int _port) : Server(_ip, _port){

}

std::string IngressProxy::getProxyType(){
    return type;
}

bool IngressProxy::loadNetConfig(std::string path, NodeType type){
    Utils utls;
    bool status = false;
    if(type == RAP)
        raps = utls.loadConfig(path, type, status);
    else if(type == INGRESS)
        ingress = utls.loadConfig(path, type, status);
    else
        return false;

    return (status == true) ? true : false;
}

int IngressProxy::lookupProxyId(){
    Utils utls;
    utls.printConfig(ingress, INGRESS);
    for(auto& i: ingress){
        if(i.second.ip == ip && i.second.port == port){
            return i.first;
        }
    }
    return 0;
}
Node IngressProxy::findNearestRap(){
    std::cout<<TAG<<"looking up nearest rap...\n";
    int id  = lookupProxyId();
    int rap_id;

    if(id > 0){
        int min_distance = INT_MAX;
        for(auto& r: raps){
            int d = abs(r.first - id);
            if(d < min_distance){
                min_distance = d;
                rap_id = r.first;
            }
        }
        return raps.at(rap_id);
    }
    Node t = {"", 0};
    return t;
}

int IngressProxy::forwardToRap(Packet *packet){
    Node n = findNearestRap();
    if(n.ip != "" || n.port != 0){
        std::cout<<TAG<<"+forwarding packet from ["<<packet->source_ip<<":"
        <<packet->source_port<<"] to closest RAP\n";
        packet->hops++;
        packet->destination_ip = n.ip;
        packet->destination_port = n.port;
        return forward(packet->destination_ip.c_str(), packet->destination_port, &packet);
    }else{
        std::cerr<<TAG<<"could not find local rendezvoux proxy...\n ";
    }
    return 0;
}

int IngressProxy::forwardToClient(Packet *packet){
    std::cout<<TAG<<"\t+forwarding packet to ["<<packet->source_ip<<":"
    <<packet->source_port<<"]\n";
    return forward(packet->source_ip.c_str(), packet->source_port, &packet);
}
