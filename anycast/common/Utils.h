/**
 * @Author: Michel Andy <andy>
 * @Date:   2017-04-16T23:11:16-04:00
 * @Email:  Andymic12@gmail.com
 * @Project: Anycast
 * @Filename: Utils.h
 * @Last modified by:   andy
 * @Last modified time: 2017-04-16T23:42:19-04:00
 */

#include<iostream>
#include<map>
#include<fstream>

struct Node{
        std::string ip;
        int port;
};

enum NodeType{
    RAP,
    INGRESS,
    JOIN,
    TARGET
};

class Utils{
private:
    const std::string TAG = "Utils\n\t+";
public:
    std::map<int, Node> loadConfig(std::string path, NodeType type, bool &status){
        std::map <int, Node> temp;
        std::ifstream conf_file(path);

        std::string ip;
        int id, port;

        if(conf_file.is_open()){
            while(conf_file >> id >> ip >> port){
                Node n = {ip, port};
                temp.insert(std::pair<int, Node>(id, n));
            }
        }else{
            std::cerr<<TAG<<"failed to load "<<path<<" config file\n";
            status = false;
        }

        status = true;
        return temp;
    }

    void printConfig(std::map<int, Node> conf, NodeType type){
        std::cout<<TAG<<"Values for "<<type<<" nodes\n";
        std::cout<<"\tNODE_ID IP \t\tPORT\n";
        for(auto& c: conf){
            std::cout<<"\t"<<c.first<<"\t"<<c.second.ip<<"\t"<<c.second.port<<std::endl;
        }
    }
};
