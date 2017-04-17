/**
 * @Author: Michel Andy <andy>
 * @Date:   2017-03-27T19:45:10-04:00
 * @Email:  Andymic12@gmail.com
 * @Project: Anycast
 * @Filename: Packet.h
 * @Last modified by:   andy
 * @Last modified time: 2017-04-16T00:36:52-04:00
 */

#ifndef __packet_h__
#define __packet_h__
#include<string>
#include<sstream>
#include<vector>

class Packet{
private:
    template<typename Out>
    void split(const std::string &value, char delimiter, Out result);
    std::vector<std::string> split(std::string value, char delimiter);
public:
    Packet(std::string value);
    Packet();
    std::string id;
    int size;
    int hops;
    std::string proxy_type;
    std::string data;
    std::string source_ip;
    int source_port;
    std::string destination_ip;
    int destination_port;
    std::string forwarder_ip;
    int forwarder_port;
    std::string to_string();
    ~Packet();
};

#endif
