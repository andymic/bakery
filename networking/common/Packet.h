/**
 * @Author: Michel Andy <andy>
 * @Date:   2017-03-27T19:45:10-04:00
 * @Email:  Andymic12@gmail.com
 * @Project: Anycast
 * @Filename: Packet.h
 * @Last modified by:   andy
 * @Last modified time: 2017-03-28T14:14:39-04:00
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
    int size;
    int hops;
    std::string data;
    std::string source_ip;
    std::string destination_ip;
    std::string to_string();
    ~Packet();
};

#endif
