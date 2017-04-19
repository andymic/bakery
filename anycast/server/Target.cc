/**
 * @Author: Michel Andy <andy>
 * @Date:   2017-04-01T17:04:11-04:00
 * @Email:  Andymic12@gmail.com
 * @Project: Anycast
 * @Filename: Target.cc
 * @Last modified by:   andy
 * @Last modified time: 2017-04-19T10:50:52-04:00
 */

#include "Target.h"

Target::Target(const char * _ip, int _port, bool _verbose): Server(_ip, _port, _verbose){

}

std::string Target::getProxyType(){
    return type;
}

int Target::reply(Packet* packet){
    packet->data = "Here is milk...";
    packet->hops++;
    return forward(packet->forwarder_ip.c_str(), packet->forwarder_port, &packet);
}
