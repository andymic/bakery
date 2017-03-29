/**
 * @Author: Michel Andy <andy>
 * @Date:   2017-03-28T13:34:55-04:00
 * @Email:  Andymic12@gmail.com
 * @Project: Anycast
 * @Filename: Packet.cc
 * @Last modified by:   andy
 * @Last modified time: 2017-03-28T18:38:26-04:00
 */

#include "Packet.h"
#include<map>
#include<vector>
#include<iterator>
#include<string>

template<typename Out>
void Packet::split(const std::string &value, char delimiter, Out result){
    std::istringstream ss;
    ss.str(value);
    std::string token;
    while(std::getline(ss, token, delimiter)){
        *(result++) = token;
    }
}

std::vector<std::string> Packet::split(std::string value, char delimiter){
        std::vector<std::string> result;
        split(value, delimiter, std::back_inserter(result));
        return result;
}

Packet::Packet(std::string value){
    std::map<std::string, std::string> props;

    std::istringstream iss(value);
    std::string token;
    char delimiter = '\n';
    while(std::getline(iss, token, delimiter)){
        std::vector<std::string> result = split(token, '=');

        if(result.size() == 2)
            props.insert(std::pair<std::string, std::string>(result.at(0), result.at(1)));
        else
            props.insert(std::pair<std::string, std::string>(result.at(0), ""));
    }

    if(props.size() > 0){
        size = std::stoi(props.find("size")->second);
        hops = std::stoi(props.find("hops")->second);
        data = props.find("data")->second;
        source_ip = props.find("source_ip")->second;
        destination_ip = props.find("destination_ip")->second;
    }
}


Packet::Packet(){
    size = 0;
    hops = 0;
    data = "";
    source_ip = "";
    destination_ip = "";
}

std::string Packet::to_string(){
    std::ostringstream oss;
    oss << "size="<< size <<std::endl;
    oss << "hops=" << hops <<std::endl;
    oss << "data=" << data <<std::endl;
    oss << "source_ip=" << source_ip <<std::endl;
    oss << "destination_ip=" << destination_ip<<std::endl;
    return oss.str();
}

Packet::~Packet(){

}
