/**
 * @Author: Michel Andy <andy>
 * @Date:   2017-04-01T16:29:20-04:00
 * @Email:  Andymic12@gmail.com
 * @Project: Anycast
 * @Filename: IngressProxy.cc
 * @Last modified by:   andy
 * @Last modified time: 2017-04-16T19:54:10-04:00
 */


#include "IngressProxy.h"
#define TAG "IngressProxy-\n"
IngressProxy::IngressProxy(const char * _ip, int _port) : Server(_ip, _port){

}

std::string IngressProxy::getProxyType(){
    return type;
}

int IngressProxy::forwardToRap(Packet *packet){
    std::cout<<TAG<<"\t+forwarding packet from ["<<packet->source_ip<<":"
    <<packet->source_port<<"]\n";
    packet->hops++;
    return forward(packet->destination_ip.c_str(), packet->destination_port, &packet);
}

int IngressProxy::forwardToClient(Packet *packet){
    std::cout<<TAG<<"\t+forwarding packet to ["<<packet->source_ip<<":"
    <<packet->source_port<<"]\n";
    return forward(packet->source_ip.c_str(), packet->source_port, &packet);
}
