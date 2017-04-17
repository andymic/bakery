/**
 * @Author: Michel Andy <andy>
 * @Date:   2017-04-01T16:24:01-04:00
 * @Email:  Andymic12@gmail.com
 * @Project: Anycast
 * @Filename: JoinProxy.cc
 * @Last modified by:   andy
 * @Last modified time: 2017-04-16T20:12:21-04:00
 */

#include "JoinProxy.h"
#define TAG "JoinProxy -\n"
JoinProxy::JoinProxy(const char *_ip, int _port): Server(_ip, _port){
    //proxy_id = id;
}

std::string JoinProxy::getProxyType(){
    return type;
}

int JoinProxy::forwardToTarget(Packet **packet){
    (*packet)->forwarder_ip = ip;
    (*packet)->forwarder_port = port;
    (*packet)->hops++;
    std::cout<<TAG<<"\t+forwarding packet from ["<<(*packet)->source_ip<<":"
    <<(*packet)->source_port<<"]\n";
    return forward((*packet)->destination_ip.c_str(), (*packet)->destination_port, packet, true);
}

int JoinProxy::reply(Packet *packet){
    std::cout<<TAG<<"\t+forwarding packet from ["<<packet->source_ip<<":"
    <<packet->source_port<<"]\n";
    packet->hops++;
    return forward(packet->source_ip.c_str(), packet->source_port, &packet);
}
