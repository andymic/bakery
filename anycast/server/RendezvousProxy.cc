/**
 * @Author: Michel Andy <andy>
 * @Date:   2017-04-01T16:27:20-04:00
 * @Email:  Andymic12@gmail.com
 * @Project: Anycast
 * @Filename: RendezvousProxy.cc
 * @Last modified by:   andy
 * @Last modified time: 2017-04-02T10:03:33-04:00
 */

#include "RendezvousProxy.h"
#define TAG "RendezvousProxy-\n"
RendezvousProxy::RendezvousProxy(const char * _ip, int _port) : Server(_ip, _port){

}

std::string RendezvousProxy::getProxyType(){
    return type;
}

int RendezvousProxy::forwardToJoin(Packet *packet){
    std::cout<<TAG<<"\t+forwarding packet from ["<<packet->source_ip<<":"
    <<packet->source_port<<"]\n";
    packet->hops++;
    return forward(packet->destination_ip.c_str(), packet->destination_port, packet);
}
