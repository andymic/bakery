/**
 * @Author: Michel Andy <andy>
 * @Date:   2017-04-01T16:24:01-04:00
 * @Email:  Andymic12@gmail.com
 * @Project: Anycast
 * @Filename: JoinProxy.cc
 * @Last modified by:   andy
 * @Last modified time: 2017-04-17T03:08:34-04:00
 */

#include "JoinProxy.h"
#define TAG "JoinProxy -\n\t+"
JoinProxy::JoinProxy(const char *_ip, int _port): Server(_ip, _port){
}

std::string JoinProxy::getProxyType(){
    return type;
}

bool JoinProxy::loadNetConfig(std::string path, NodeType type){
    Utils utls;
    bool status = false;
    if(type == TARGET)
        targets = utls.loadConfig(path, type, status);
    else if(type == JOIN)
        joins = utls.loadConfig(path, type, status);
    else
        return false;

    return (status == true) ? true : false;
}

Node JoinProxy::findLocalTarget(){
    std::map<int, Node>::iterator it;
    Utils utils;
    int id;

    for(auto& j: joins){
        //looking up the id of the join proxy to calculate the distance

        std::cout<<TAG<<ip<<port<< " conf: "<<j.second.ip<<j.second.port<<std::endl;
        if(j.second.ip == ip && j.second.port == port){
            id = j.first;
        }
    }
    std::cout<<TAG<<"server id: "<<id<<std::endl;
    it = targets.find(id);

    if(it == targets.end()){
        std::cerr<<TAG<<"could not find local target!!\n";
        Node t = {"", 0};
        return t;
    }
    return targets.at(id);
}
int JoinProxy::forwardToTarget(Packet **packet){
    Node local_target = findLocalTarget();
    if(local_target.ip == "" && local_target.port == 0){
        std::cout<<TAG<<"could not find local target!!\n";
        return 0;
    }

    (*packet)->proxy_type = "JAP";
    (*packet)->destination_ip = local_target.ip;
    (*packet)->destination_port = local_target.port;
    (*packet)->forwarder_ip = ip;
    (*packet)->forwarder_port = port;
    (*packet)->hops++;
    std::cout<<TAG<<"\t+forwarding packet from ["<<(*packet)->source_ip<<":"
    <<(*packet)->source_port<<"] "<<(*packet)->proxy_type<<"\n";
    return forward((*packet)->destination_ip.c_str(), (*packet)->destination_port, packet, true);
}

int JoinProxy::reply(Packet *packet){
    std::cout<<TAG<<"\t+forwarding packet from ["<<packet->source_ip<<":"
    <<packet->source_port<<"]\n";
    packet->hops++;
    return forward(packet->source_ip.c_str(), packet->source_port, &packet);
}
